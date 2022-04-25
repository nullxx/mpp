#!/bin/bash

docker build -t mpp-core -f Dockerfile.build.core .

docker cp $(docker create --rm mpp-core):/mpp-cpu/output/. mpp-front/src/lib/core/files

docker rmi $(docker images | grep 'mpp-core')

# --no-cache to prevent using old core/files
docker build --no-cache -t mpp-front mpp-front

echo "Frontend built on mpp-front"
echo "Build finished"