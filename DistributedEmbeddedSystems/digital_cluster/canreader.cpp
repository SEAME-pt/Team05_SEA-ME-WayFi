#include "canreader.h"

CanReader::CanReader(QObject *parent)
    : QObject{parent} {
    running = true;
}

CanReader::~CanReader() {
    if (can_fd != -1) {
        close(can_fd);
    }
}

void CanReader::stop_reading() {
    running = false;  
}

void CanReader::start_reading(CustomDial *dial) {
    can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_fd == -1) {
        std::cerr << "Error opening CAN socket" << std::endl;
        return;
    }

    struct ifreq ifr;
    QString can = "can0";
    strncpy(ifr.ifr_name, can.toStdString().c_str(), IFNAMSIZ);
    ioctl(can_fd, SIOCGIFINDEX, &ifr);
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding CAN socket" << std::endl;
        return;
    }
    fcntl(can_fd, F_SETFL, O_NONBLOCK);

    while (running) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(can_fd, &rfds);

        struct timeval tv;
        tv.tv_sec = 1;  // Timeout of 1 second
        tv.tv_usec = 0;
        // Use select to check for activity on CAN socket
        int retval = select(can_fd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            std::cerr << "Error in select" << std::endl;
            break;
        }
        
        if (retval) {
            // There is data to read
            struct can_frame frame;
            int nbytes = read(can_fd, &frame, sizeof(struct can_frame));
            if (nbytes < 0) {
                std::cerr << "Error reading CAN message" << std::endl;
                break;
            }

            std::cout << "Received CAN ID: 0x" << std::hex << frame.can_id << " Data: ";
            for (int i = 0; i < frame.can_dlc; ++i) {
                std::cout << "0x" << std::hex << static_cast<int>(frame.data[i]) << " ";
            }
            std::cout << std::dec << std::endl;  // Reset to decimal format

            if (frame.can_id == 0x123 || frame.can_id == 0x200) {  // Example CAN ID for speed
                dial->set_current(static_cast<int>(frame.data[0]));
            
        }}
        else 
            std::cout << "Can timeout\n";
            
        QCoreApplication::processEvents();
    }
}