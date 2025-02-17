#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

const std::string CAN_INTERFACE = "can0";
const int BITRATE = 500000; // CAN interface bitrate
const int MONITOR_INTERVAL = 5; // Monitoring interval in seconds

// Function to execute system commands
int executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error executing command: " << command << std::endl;
    }
    return result;
}

// Activates the CAN interface
void activateCAN() {
    std::cout << "Activating CAN interface (" << CAN_INTERFACE << ") with bitrate " << BITRATE << "..." << std::endl;
    
    // Deactivate the interface if it's already active
    std::string cmd = "sudo ip link set " + CAN_INTERFACE + " down";
    executeCommand(cmd);

    // Configure the interface with the desired bitrate
    cmd = "sudo ip link set " + CAN_INTERFACE + " type can bitrate " + std::to_string(BITRATE);
    if (executeCommand(cmd) == 0) {
        cmd = "sudo ip link set " + CAN_INTERFACE + " up";
        executeCommand(cmd);
    }
}

// Deactivates the CAN interface
void deactivateCAN() {
    std::cout << "Deactivating CAN interface (" << CAN_INTERFACE << ")..." << std::endl;
    std::string cmd = "sudo ip link set " + CAN_INTERFACE + " down";
    executeCommand(cmd);
}

// Monitors the CAN interface
void monitorCAN() {
    std::cout << "Starting monitoring of CAN interface (" << CAN_INTERFACE << ")..." << std::endl;
    while (true) {
        std::string statusCmd = "ip link show " + CAN_INTERFACE + " | grep -q 'state UP'";
        if (executeCommand(statusCmd) != 0) {
            std::cerr << "CAN interface is down. Attempting to reactivate..." << std::endl;
            activateCAN();
        }
        std::this_thread::sleep_for(std::chrono::seconds(MONITOR_INTERVAL));
    }
}

// Signal handler for SIGTERM
void signalHandler(int signum) {
    if (signum == SIGTERM) {
        std::cout << "Received SIGTERM. Cleaning up and deactivating CAN interface..." << std::endl;
        deactivateCAN();
        std::exit(0); // Exit gracefully
    }
}

int main() {
    // Register the signal handler
    std::signal(SIGTERM, signalHandler);

    // Activate the CAN interface
    activateCAN();

    // Start monitoring the CAN interface
    monitorCAN();

    return 0;
}

