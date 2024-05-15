# Use Ubuntu 16.04 as the base image
FROM ubuntu:16.04

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install necessary tools for the toolchain
RUN apt-get update && apt-get install -y \
    build-essential \
    automake \ 
    cmake \
    make \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    libc6-dev-arm64-cross \
    wget \ 
    mc
    
# Set up working directory
WORKDIR /workspace

# Copy the build script into the image
COPY media/3rd-party/configure.sh /workspace/configure.sh
# Make the script executable and run it
RUN chmod +x /workspace/configure.sh && /workspace/configure.sh

