FROM arm64v8/ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    apt-get update --fix-missing

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libopencv-dev \
    curl \
    qtbase5-dev \
    qt5-qmake \
    qtbase5-dev:arm64 \
    qt5-qmake:arm64 \
    libqt5widgets5:arm64

RUN apt-get install -y \
    qtchooser \
    qtbase5-dev-tools 
#     libqt5mqtt5-dev \

# RUN wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key && \
#     apt-key add mosquitto-repo.gpg.key && \
#     cd /etc/apt/sources.list.d/ && \
#     wget http://repo.mosquitto.org/debian/mosquitto-jammy.list && \
#     apt-get update && apt-get install mosquitto mosquitto-clients

# RUN apt-get install -y wget && wget -qO - http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key | tee /etc/apt/trusted.gpg.d/mosquitto.asc && \
#     echo "deb http://repo.mosquitto.org/debian buster main" | tee /etc/apt/sources.list.d/mosquitto.list && \
#     apt-get update && \
#     apt-get install -y mosquitto mosquitto-clients && \
#     apt-get clean

# RUN apt-get update && apt-get install -y qemu binfmt-support qemu-user-static \
#     update-binfmts --enable qemu-aarch64 

# RUN apt-get install -y libqt5mqtt5-dev:arm64

# RUN apt-get update && apt-get install -y \
#     qemu \
#     binfmt-support \
#     qemu-user-static

# RUN update-binfmts --enable qemu-aarch64
RUN git clone https://code.qt.io/qt/qtmqtt.git /qtmqtt && \
    cd /qtmqtt && \
    git checkout 5.15 && \
    qmake "CONFIG+=qt5" && \ 
    make && \
    make install

# RUN rm -rf qt-everywhere-src-5.15.2 && \
#     curl -SL https://download.qt.io/archive/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.tar.xz \
#     -o qt-everywhere-src.tar.xz && \
#     tar -xf qt-everywhere-src.tar.xz && \
#     cd qt-everywhere-src-5.15.2 && \
#     ./configure -opensource -confirm-license -release -nomake \
#     examples -nomake tests -qt-mqtt -skip qt3d -skip qtwebengine \
#     -platform linux-g++ -c++std c++17 \
#     -I/usr/aarch64-linux-gnu/include/c++/10 \
#     -I/usr/aarch64-linux-gnu/include/ && \
#     make -j1 && \
#     make install && \
#     cd .. && rm -rf qt-everywhere-src*

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# RUN echo "QT_SELECT=5" >> /etc/environment

#Set working directory to /workspace
WORKDIR /workspace

#Copy local project files into the container
COPY . /workspace

#Build project
RUN cd build && \
    rm -rf CMakeCache.txt CMakeFiles/ && \
    cmake .. && \
    make clean && \
    make && \
    chmod +x ./digital_cluster

ENTRYPOINT ["./build/digital_cluster"]
