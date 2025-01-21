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
#define TOPIC_TEMPERATURE "jetracer/temperature"
#define USERNAME "jetracer"
#define PASSWORD "Ft_seame5"

struct mosquitto *local_mosq, *cloud_mosq;

// Cleanup function to shut down resources
void cleanUp(int signum) {
    std::cout << "\nShutting down temperature republisher...\n";

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
    std::cout << "Temperature republisher stopped.\n";
    exit(signum);
}

// Callback for connection
void onConnect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        std::cout << "Connected to MQTT broker.\n";

        // Re-subscribe to topics after reconnection
        if (mosquitto_subscribe(mosq, NULL, TOPIC_TEMPERATURE, 0) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to re-subscribe to topic: " << TOPIC_TEMPERATURE << "\n";
        } else {
            std::cout << "Re-subscribed to topic: " << TOPIC_TEMPERATURE << "\n";
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
        int ret = mosquitto_publish(cloud_mosq, NULL, TOPIC_TEMPERATURE, message.size(), message.c_str(), 0, false);
        if (ret != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to publish to cloud broker: " << mosquitto_strerror(ret) << "\n";
        } else {
            std::cout << "Forwarded message to cloud broker: " << message << "\n";
        }
    }
}

// Setup function to connect to an MQTT broker
void setupBroker(struct mosquitto **mosq, const std::string &id, const std::string &broker, int port, bool useTLS = false, const std::string &username = "", const std::string &password = "") {
    *mosq = mosquitto_new(id.c_str(), true, NULL);
    if (!*mosq) {
        throw std::runtime_error("Error creating MQTT client");
    }

    // Set credentials for cloud broker
    if (!username.empty() && !password.empty()) {
        mosquitto_username_pw_set(*mosq, username.c_str(), password.c_str());
    }

    // Configure TLS if required
    if (useTLS) {
        if (mosquitto_tls_set(*mosq, "/etc/ssl/certs/ca-certificates.crt", NULL, NULL, NULL, NULL) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to configure TLS");
        }
    }

    // Enable automatic reconnection
    mosquitto_reconnect_delay_set(*mosq, 1, 10, true);

    // Connect to the broker with retries
    while (mosquitto_connect(*mosq, broker.c_str(), port, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Unable to connect to broker at " << broker << ". Retrying in 1 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Connected to broker at " << broker << "\n";
}

int main() {
    signal(SIGINT, cleanUp);
    signal(SIGTERM, cleanUp);

    mosquitto_lib_init();

    try {
        // Setup local MQTT broker
        setupBroker(&local_mosq, "TemperatureSubscriberLocal", LOCAL_BROKER, LOCAL_PORT);

        // Setup cloud MQTT broker
        setupBroker(&cloud_mosq, "TemperaturePublisherCloud", CLOUD_BROKER, CLOUD_PORT, true, USERNAME, PASSWORD);

        // Subscribe to the temperature topic on the local broker
        mosquitto_connect_callback_set(local_mosq, onConnect);
        mosquitto_disconnect_callback_set(local_mosq, onDisconnect);
        mosquitto_message_callback_set(local_mosq, onMessage);

        // Start the Mosquitto loops
        if (mosquitto_loop_start(local_mosq) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to start loop for local broker");
        }
        if (mosquitto_loop_start(cloud_mosq) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Failed to start loop for cloud broker");
        }

        std::cout << "Temperature republisher running. Subscribed to local broker and publishing to cloud broker.\n";

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
