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

*The following commands assume that the DePINC wallet is running on your localhost, listening on port 8732 (the default RPC port). The following two sections provide two ways to grant access permission from the DePINC wallet RPC service.*

### Prefer to use a custom username and password

If you prefer to use a custom username and password, please use the following command instead:

```bash
docker run --network host -v ./:/mnt -p 19191:19191 --name timelord depinc/timelord:latest --mainnet --bind 0.0.0.0 --rpc localhost:8732 --rpc-user <your-username> --rpc-password <your-password>
```

Replace `<your-username>` and `<your-password>` with your desired username and password.

### Prefer to use the cookie file

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

## Test VDF speed

To test your computer's power, please run the vdf bench program. It's already packed in the docker image, please run the following command:

```bash
docker run --entrypoint "" -it depinc/timelord /vdf_bench square_asm 1000000
```

The result looks like the following, which means your computer can solve VDF iterations at a rate of 78.6K per second:

```
Time: 12707 ms; n_slow: 213; speed: 78.6K ips
a = 0x75cae01275396eecb97f56761eaa431fda912c8f06467f0e04c34cdc250bfde37403a4d8287f128d9d0e0fec84e80b30892ec24ae385727fc95f8d1192a9314
b = -0x8c5609e0d6a8d23874e25334dfeae89121739659d6312b4b5a6b7d96a7d9e3478bd21bc34f2288b67f1ea785fecc40c8bbfb4301ccec012b7d9a7e3f2e698d
c = 0x6d345879ff552bb1d1b7ca7b2fbcb07c5dddc25728ed1b35263b6b1354a79bcf546ddfed9d13e7a35c335a57ac9e367f332bfb6707c0983edff5d7b75f99f1834
```

## Build docker image locally

*You don't need to build your own image, pulling it from hub.docker.com is enough for common users.*

To build the docker image, just run the following command:

```bash
docker build . -t timelord --network host
```

If you want to build the docker image through a proxy server, let's say it is 'http://192.168.0.200', you can run the following command:

```bash
docker build . -t timelord --network host --build-arg PROXY=http://192.168.0.200:8080
```
