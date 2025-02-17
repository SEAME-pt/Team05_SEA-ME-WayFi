#include <iostream>
#include <fstream>
#include <string>
#include <mosquitto.h>
#include <csignal>
#include <thread>
#include <chrono>

#define LOCAL_BROKER "127.0.0.1"
#define LOCAL_PORT 1883
#define TOPIC_TEMPERATURE "jetracer/temperature"

struct mosquitto *local_mosq;

// Cleanup function to shut down resources
void cleanUp(int signum) {
    std::cout << "\nShutting down temperature publisher...\n";

    if (local_mosq) {
        mosquitto_disconnect(local_mosq);
        mosquitto_destroy(local_mosq);
    }

    mosquitto_lib_cleanup();
    std::cout << "Temperature publisher stopped.\n";
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

// Function to read the Raspberry Pi CPU temperature
double getTemperature() {
    try {
        std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
        if (!tempFile.is_open()) {
            throw std::ios_base::failure("Failed to open temperature file");
        }
        int tempMilliCelsius;
        tempFile >> tempMilliCelsius;
        return tempMilliCelsius / 1000.0;
    } catch (const std::exception &e) {
        std::cerr << "Error reading temperature: " << e.what() << "\n";
        return -1.0;
    }
}

int main() {
    signal(SIGINT, cleanUp);
    signal(SIGTERM, cleanUp);

    mosquitto_lib_init();

    // Initialize MQTT client for local broker
    local_mosq = mosquitto_new("TemperaturePublisherLocal", true, NULL);
    if (!local_mosq) {
        std::cerr << "Error creating local MQTT client\n";
        return 1;
    }

    // Enable automatic reconnection
    mosquitto_reconnect_delay_set(local_mosq, 1, 10, true);

    // Connect to the local MQTT broker
    while (mosquitto_connect(local_mosq, LOCAL_BROKER, LOCAL_PORT, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Unable to connect to local broker. Retrying in 1 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Connected to local MQTT broker.\n";

    // Start the Mosquitto loop in the background
    if (mosquitto_loop_start(local_mosq) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Failed to start MQTT loop for local broker\n";
        return 1;
    }

    try {
        while (true) {
            // Read the temperature
            double temperature = getTemperature();
            if (temperature < 0) {
                std::cerr << "Skipping publishing due to invalid temperature.\n";
                continue;
            }

            // Publish temperature to the local MQTT broker
            std::string tempStr = std::to_string(temperature);
            int ret = mosquitto_publish(local_mosq, NULL, TOPIC_TEMPERATURE, tempStr.size(), tempStr.c_str(), 0, false);
            if (ret != MOSQ_ERR_SUCCESS) {
                std::cerr << "Failed to publish temperature: " << mosquitto_strerror(ret) << "\n";
            } else {
                std::cout << "Published temperature: " << temperature << "°C to local broker\n";
            }

            // Wait for 5 seconds before the next update
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in main loop: " << e.what() << "\n";
    }

    cleanUp(0);  // Clean up resources
    return 0;
}
