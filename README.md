# Timelord

Provides a way to run the VDF calculator on a single machine or in a Docker container.

## System requirements

You can run Timelord under these operating systems. We highly recommend you run your own Timelord to obtain time proofs.

* Linux/macOS - Native/Docker

* Windows - Docker

## Installation

Please refer to the [wiki](https://github.com/depinc/timelord/wiki/Install-and-Run) to learn how to run a Timelord on your computer.

## Running with Docker

*Please note: First, you need to install Docker on your system. Visit [docker.io](https://www.docker.com/) for more information.*

1. The Docker image is available on [Docker Hub](https://hub.docker.com/r/depinc/timelord). Follow these instructions to pull the Docker image to your local machine:

```bash
# Pull the latest Timelord Docker image
docker pull depinc/timelord:latest
```

2. Start the Timelord container using the `docker run` command. The Docker image includes all necessary binaries to run Timelord on your local system. Use the following command to start the container:

```bash
docker run --network host -v ./:/mnt -p 19191:19191 --name timelord depinc/timelord:latest --mainnet --bind 0.0.0.0 --rpc localhost:8732 --rpc-user rpcuser --rpc-password rpcpassword
```

The command assumes that the Depinc wallet is running on your localhost, listening on port 8732 (the default RPC port), with the username 'rpcuser' and password 'rpcpassword'.

If you prefer to use a cookie file from '$HOME/.depinc/.cookie', please use the following command instead:

```bash
docker run --network host -v ./:/mnt -v $HOME/.depinc/.cookie:/.cookie -p 19191:19191 --name timelord depinc/timelord:latest --mainnet --bind 0.0.0.0 --rpc localhost:8732 --use-cookie --cookie /.cookie
```

## Modify miner's configuration file

Now you can configure your miner program to connect to the Timelord by adding the following:

```jsonc
{
    // ...
    "timelords": [
        "localhost:19191"
    ],
}

```
