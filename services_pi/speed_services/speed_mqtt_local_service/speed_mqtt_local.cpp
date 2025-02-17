#include <iostream>
#include <cstring>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <csignal>
#include <iomanip>
#include <sstream>
#include <mosquitto.h>
#include <thread>
#include <chrono>

#define LOCAL_BROKER "127.0.0.1"
#define LOCAL_PORT 1883
#define TOPIC_SPEED "jetracer/speed"
#define CAN_INTERFACE "can0"

int socket_can;
struct mosquitto *local_mosq;

// Cleanup function to shut down resources
void cleanUp(int signum) {
    std::cout << "\nShutting down speed publisher...\n";

    // Close the CAN socket
    if (socket_can >= 0) {
        close(socket_can);
    }

    if (local_mosq) {
        int ret = mosquitto_disconnect(local_mosq);
        if (ret != MOSQ_ERR_SUCCESS) {
            std::cerr << "Error disconnecting from local broker: " << mosquitto_strerror(ret) << "\n";
        }
        mosquitto_destroy(local_mosq);
    }

    mosquitto_lib_cleanup();
    std::cout << "Speed publisher stopped.\n";
    exit(signum);
}


void onConnect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        std::cout << "Successfully connected to MQTT broker.\n";
    } else {
        std::cerr << "Failed to connect to MQTT broker: " << mosquitto_strerror(rc) << "\n";
    }
}

// MQTT disconnection callback
void onDisconnect(struct mosquitto *mosq, void *obj, int rc) {
    std::cerr << "Disconnected from MQTT broker. Reason: " << mosquitto_strerror(rc) << "\n";
}

int main() {
    struct ifreq ifr;
    struct sockaddr_can addr;

    // Handle termination signals
    signal(SIGINT, cleanUp);
    signal(SIGTERM, cleanUp);

    // Configure the CAN socket
    std::cout << "Setting up CAN socket...\n";
    if ((socket_can = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error creating CAN socket");
        return 1;
    }

    // Apply CAN frame filtering
    struct can_filter rfilter[1];
    rfilter[0].can_id = 0x100;         // Filter for speed sensor (ID 0x100)
    rfilter[0].can_mask = CAN_SFF_MASK;

    if (setsockopt(socket_can, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0) {
        perror("Error setting CAN filter");
        return 1;
    }

    // Bind the socket to the CAN interface
    std::strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(socket_can, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_can, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding CAN socket");
        return 1;
    }

    std::cout << "CAN socket configured. Starting MQTT...\n";

    // Initialize MQTT library
    mosquitto_lib_init();

    // Configure the local MQTT broker
    local_mosq = mosquitto_new("jetracer_local", true, NULL);
    if (!local_mosq) {
        std::cerr << "Error creating local MQTT client\n";
        return 1;
    }

    // Set connection and disconnection callbacks
    mosquitto_connect_callback_set(local_mosq, onConnect);
    mosquitto_disconnect_callback_set(local_mosq, onDisconnect);

    // Enable automatic reconnection
    mosquitto_reconnect_delay_set(local_mosq, 1, 10, true);

    // Attempt to connect to the broker
    while (mosquitto_connect(local_mosq, LOCAL_BROKER, LOCAL_PORT, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Unable to connect to local MQTT broker. Retrying in 1 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Connected to local MQTT broker. Listening on CAN...\n";

    struct can_frame frame;
    while (true) {
        ssize_t nbytes = read(socket_can, &frame, sizeof(struct can_frame));

        if (nbytes > 0 && frame.can_id == 0x100 && frame.can_dlc >= 2) {
            int speed_scaled = (frame.data[0] << 8) | frame.data[1];
            double speed_kmh = speed_scaled / 100.0;

            std::ostringstream stream;
            stream << std::fixed << std::setprecision(2) << speed_kmh;
            std::string message = stream.str();

            // Publish speed to the local MQTT broker
            int ret = mosquitto_publish(local_mosq, NULL, TOPIC_SPEED, message.size(), message.c_str(), 0, false);
            if (ret == MOSQ_ERR_NO_CONN) {
                std::cerr << "MQTT broker disconnected. Message no sent.\n";
            } else if (ret != MOSQ_ERR_SUCCESS) {
                std::cerr << "Error publishing message: " << mosquitto_strerror(ret) << "\n";
            } else {
                std::cout << "Speed: " << message << " m/s published to local broker\n";
            }
        }

	mosquitto_loop(local_mosq, 0, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Prevent high CPU usage
    }

    return 0;
}
