# Docker Build Setup for webserv

This Docker Compose configuration allows you to compile webserv on macOS using a Linux container.

## Prerequisites

- [Docker Desktop](https://www.docker.com/products/docker-desktop) installed and running on your Mac

## Quick Start

### Build the project

```bash
docker-compose up build
```

This will:
1. Create a Linux container with C++20 build tools
2. Mount your project directory
3. Run `make clean && make`
4. Keep the container running with a bash shell

### Compile after changes

Once the container is running, you can execute make commands directly:

```bash
docker-compose exec build make
```

Or to clean and rebuild:

```bash
docker-compose exec build make re
```

### Stop containers

```bash
docker-compose down
```

## Running the compiled binary

After compilation completes, you can run the server:

```bash
docker-compose up run
```

Then in the container:

```bash
./webserv configs/default.conf
```

## Accessing container shell

While containers are running:

```bash
# Access the build container
docker-compose exec build bash

# Access the run container  
docker-compose exec run bash
```

## How it works

- **Dockerfile**: Builds a Linux (Ubuntu 22.04) environment with C++20 compiler and make
- **docker-compose.yml**: Creates two services:
  - `build`: Compiles your code
  - `run`: Runs the compiled binary with exposed ports
- **.dockerignore**: Prevents unnecessary files from being copied into the container

## Troubleshooting

- If Docker containers won't start, ensure Docker Desktop is running
- If compilation fails, ensure your project has all required files
- Use `docker-compose logs` to see output from services
- Use `docker-compose down -v` to remove containers and volumes if you want a clean rebuild

## Networking

When running the server in the container, you can access it from your Mac at `localhost:8080` (or whatever port you configure).
