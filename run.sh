#!/bin/bash +x

# Check if the Docker image already exists
if ! docker image ls | grep -q "arm-a53-toolchain" ; then
  docker build -t arm-a53-toolchain:latest .
fi

if [ "$1" ] && [ -d "media/$1" ]; then
	docker run -it -v "$(pwd)/media:/workspace" arm-a53-toolchain:latest /bin/bash -c "./build.sh \"$1\""
else
	docker run -it -v "$(pwd)/media:/workspace" arm-a53-toolchain:latest /bin/bash
fi

