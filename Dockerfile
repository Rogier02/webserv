FROM ubuntu:22.04

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    gdb \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Build the project
RUN make

# Expose port 
EXPOSE 8080

# Run the server
CMD ["./webserv"]