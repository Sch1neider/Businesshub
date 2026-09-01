FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg

RUN /opt/vcpkg/bootstrap-vcpkg.sh

COPY . .

RUN /opt/vcpkg/vcpkg install crow curl --triplet x64-linux

RUN cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DVCPKG_TARGET_TRIPLET=x64-linux

RUN cmake --build build --config Release

CMD ["./build/BussinessHub"]