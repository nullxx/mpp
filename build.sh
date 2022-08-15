#!/bin/bash
_PREFIX=mpp
CORE_CONTAINER_NAME="${_PREFIX}-core"
FRONT_CONTAINER_NAME="${_PREFIX}-front"
docker build -t $CORE_CONTAINER_NAME -f Dockerfile.build.core .

docker cp $(docker create --rm $CORE_CONTAINER_NAME):/mpp-cpu/output/. $FRONT_CONTAINER_NAME/src/lib/core/files

docker rmi $(docker images '$CORE_CONTAINER_NAME' -a -q) -f

# --no-cache to prevent using old core/files
docker build --no-cache -t $FRONT_CONTAINER_NAME $FRONT_CONTAINER_NAME

echo "Frontend built on $FRONT_CONTAINER_NAME"
echo "Build finished"
