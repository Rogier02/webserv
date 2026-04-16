# Build environment for webserv on Linux
FROM ubuntu:22.04

WORKDIR /webserv

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

# Copy the project
COPY . .

# Default command runs make
CMD ["make"]
