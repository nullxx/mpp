#!/bin/bash
set -e # exit on error

_PREFIX=mpp
CORE_CONTAINER_NAME="${_PREFIX}-core"
FRONT_CONTAINER_NAME="${_PREFIX}-front"
TARGET_PLATFORMS="linux/amd64,linux/arm/v7,linux/arm64/v8,linux/386,linux/ppc64le,linux/s390x"
REPO="nullx0/mpp-front"
VERSION=$(<VERSION)

echo "This will build and push the following image to docker hub: $REPO:$VERSION"

# build frontend files
docker build -t $FRONT_CONTAINER_NAME .
docker cp $(docker create --rm $FRONT_CONTAINER_NAME):/app/build/. ./mpp-front/build-out

# build the server for all platforms
docker buildx build --platform $TARGET_PLATFORMS --push -t $REPO:$VERSION -t $REPO:latest $FRONT_CONTAINER_NAME -f $FRONT_CONTAINER_NAME/Dockerfile.final

# remove dirt
rm -rf ./mpp-front/build-out &> /dev/null
# docker rmi $(docker images "$CORE_CONTAINER_NAME" -a -q) -f &> /dev/null
# docker rmi $(docker images "$REPO:latest" -a -q) -f &> /dev/null
# rm -rf ./mpp-front/build-out &> /dev/null