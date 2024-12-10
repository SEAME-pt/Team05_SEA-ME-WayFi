#include "canreader.h"

CanReader::CanReader(QObject *parent)
    : QObject{parent}
{
}

CanReader::~CanReader() {
    if (can_fd != -1) {
        close(can_fd);
    }
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
    while (true) {
        struct can_frame frame;
        int nbytes = read(can_fd, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            std::cerr << "Error reading CAN message" << std::endl;
            break;
        }
        if (frame.can_id == 0x123 || frame.can_id == 0x200) {  // Example CAN ID for speed
            int speed = frame.data[0]; // Assuming speed is in frame.data[0]
            dial->set_current(speed);
        }
    }
}
