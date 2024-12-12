# Use a base image with Qt pre-installed
FROM ubuntu:latest

# Set non-interactive mode for package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt-get update && \
    apt-get install -y \
    gcc-13 g++-13 \
    libc6 \
    libstdc++6 \
    libqt5core5a \
    libqt5gui5 \
    libqt5widgets5 \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    build-essential \
    can-utils \
    libsocketcan-dev \
    git \
    libx11-xcb1 \
    libxcb1 \
    libxcb-util1 \
    libxcb-render0 \
    libxcb-shape0 \
    libxcb-xfixes0 \
    libxcb-sync1 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 && \
    apt-get clean

#sudo ip link set can0 up type can bitrate 500000

RUN mkdir /app

# Set the working directory in the container
WORKDIR /app

# Copy your Qt application and scripts
COPY ./build/digital_cluster /app/
RUN chmod +x /app/digital_cluster

# RUN mkdir -p build && \
#     cd build && \
#     cmake .. && \
#     make

# ENV QT_X11_NO_MITSHM=1
# ENV DISPLAY=:0  
#Use host display (to be overridden by the user at runtime)

# Set the default command to run your application
CMD ["./digital_cluster", "-lpcan"]
