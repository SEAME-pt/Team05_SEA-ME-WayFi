FROM arm64v8/ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV CROSS_COMPILE=aarch64-linux-gnu-
ENV CC=${CROSS_COMPILE}gcc
ENV CXX=${CROSS_COMPILE}g++

RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    apt-get update --fix-missing

# Add ARM64 architecture and install dependencies
RUN dpkg --add-architecture arm64 && apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libopencv-dev \
    curl \
    qtbase5-dev \
    qt5-qmake \
    qtbase5-dev:arm64 \
    qt5-qmake:arm64 \
    libqt5widgets5:arm64 && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory to /workspace
WORKDIR /workspace

# Copy the local project files into the container
COPY . /workspace
COPY ./motors /workspace/motors

# Build the project
RUN chmod +x ./motors/motors.py && \
    chmod 755 ./motors && \
    chmod +x ./motors/venv/bin/python && \ 
    cd build && \
    rm -rf CMakeCache.txt CMakeFiles/ && \
    cmake .. && \
    make clean && \
    make && \
    chmod +x ./digital_cluster

# Set entrypoint to run the application
ENTRYPOINT ["./build/digital_cluster"]
