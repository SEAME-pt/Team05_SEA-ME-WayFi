#include <iostream>
#include <mosquitto.h>
#include <csignal>
#include <string>
#include <thread>
#include <chrono>

#define LOCAL_BROKER "127.0.0.1"
#define LOCAL_PORT 1883
#define CLOUD_BROKER "972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud"
#define CLOUD_PORT 8883
#define TOPIC_SPEED "jetracer/speed"
#define USERNAME "jetracer"
#define PASSWORD "Ft_seame5"

struct mosquitto *local_mosq, *cloud_mosq;

// Cleanup function to shut down resources
void cleanUp(int signum) {
    std::cout << "\nShutting down speed republisher...\n";

    if (local_mosq) {
        int ret = mosquitto_disconnect(local_mosq);
        if (ret != MOSQ_ERR_SUCCESS) {
            std::cerr << "Error disconnecting from local broker: " << mosquitto_strerror(ret) << "\n";
        }
        mosquitto_destroy(local_mosq);
    }
    if (cloud_mosq) {
        int ret = mosquitto_disconnect(cloud_mosq);
        if (ret != MOSQ_ERR_SUCCESS) {
            std::cerr << "Error disconnecting from cloud broker: " << mosquitto_strerror(ret) << "\n";
        }
        mosquitto_destroy(cloud_mosq);
    }

    mosquitto_lib_cleanup();
    std::cout << "Speed republisher stopped.\n";
    exit(signum);
}

// Callback for connection
void onConnect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        std::cout << "Successfully connected to MQTT broker.\n";

        // Re-subscribe to the topic after reconnection
        if (mosquitto_subscribe(mosq, NULL, TOPIC_SPEED, 0) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to re-subscribe to topic: " << TOPIC_SPEED << "\n";
        } else {
            std::cout << "Re-subscribed to topic: " << TOPIC_SPEED << "\n";
        }
    } else {
        std::cerr << "Failed to connect to MQTT broker: " << mosquitto_strerror(rc) << "\n";
    }
}

// Callback for disconnection
void onDisconnect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        std::cout << "Disconnected from MQTT broker cleanly.\n";
    } else {
        std::cerr << "Unexpected disconnection from MQTT broker. Reason: " << mosquitto_strerror(rc) << "\n";
    }
}

// Callback for messages received from the local broker
void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    if (msg && msg->payload) {
        // Convert payload to a string
        std::string message(static_cast<char*>(msg->payload), msg->payloadlen);

        // Publish the message to the cloud broker
        int ret = mosquitto_publish(cloud_mosq, NULL, TOPIC_SPEED, message.size(), message.c_str(), 0, false);
        if (ret != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to publish to cloud broker: " << mosquitto_strerror(ret) << "\n";
        } else {
            std::cout << "Forwarded message to cloud broker: " << message << "\n";
        }
    }
}

int main() {
    signal(SIGINT, cleanUp);
    signal(SIGTERM, cleanUp);

    mosquitto_lib_init();

    try {
        // Configure local MQTT client
        local_mosq = mosquitto_new("local_subscriber", true, NULL);
        if (!local_mosq) {
            throw std::runtime_error("Error creating local MQTT client");
        }

        // Set local broker callbacks
        mosquitto_connect_callback_set(local_mosq, onConnect);
        mosquitto_disconnect_callback_set(local_mosq, onDisconnect);
        mosquitto_message_callback_set(local_mosq, onMessage);

        // Enable automatic reconnection for local broker
        mosquitto_reconnect_delay_set(local_mosq, 1, 30, true);

        // Connect to local broker with retries
        while (mosquitto_connect(local_mosq, LOCAL_BROKER, LOCAL_PORT, 60) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Unable to connect to local broker. Retrying in 1 second...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "Connected to local MQTT broker.\n";

        // Configure cloud MQTT client
        cloud_mosq = mosquitto_new("cloud_publisher", true, NULL);
        if (!cloud_mosq) {
            throw std::runtime_error("Error creating cloud MQTT client");
        }

        // Set cloud broker credentials
        mosquitto_username_pw_set(cloud_mosq, USERNAME, PASSWORD);

        // Configure TLS for the cloud broker
        if (mosquitto_tls_set(cloud_mosq, "/etc/ssl/certs/ca-certificates.crt", NULL, NULL, NULL, NULL) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to configure TLS for cloud MQTT broker");
        }

        // Enable automatic reconnection for cloud broker
        mosquitto_reconnect_delay_set(cloud_mosq, 1, 30, true);

        // Connect to cloud broker with retries
        while (mosquitto_connect(cloud_mosq, CLOUD_BROKER, CLOUD_PORT, 60) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Unable to connect to cloud broker. Retrying in 1 second...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "Connected to cloud MQTT broker.\n";

        // Start MQTT loops
        if (mosquitto_loop_start(local_mosq) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to start loop for local broker");
        }
        if (mosquitto_loop_start(cloud_mosq) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to start loop for cloud broker");
        }

        std::cout << "Cloud publisher running. Subscribed to local broker and publishing to cloud broker.\n";

        // Keep the program running
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        cleanUp(1);
    }

    return 0;
}
