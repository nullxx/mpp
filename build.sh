#!/bin/bash
set -e # exit on error

_PREFIX=mpp
CORE_CONTAINER_NAME="${_PREFIX}-core"
FRONT_CONTAINER_NAME="${_PREFIX}-front"
TARGET_PLATFORMS="linux/amd64,linux/arm/v7,linux/arm64/v8,linux/386,linux/ppc64le,linux/s390x"
REPO="nullx0/mpp-front"
VERSION=$(<VERSION)

echo "This will build and push the following image to docker hub: $REPO:$VERSION"

# build core
docker build -t $CORE_CONTAINER_NAME -f Dockerfile.build.core .
docker cp $(docker create --rm $CORE_CONTAINER_NAME):/mpp-cpu/output/. $FRONT_CONTAINER_NAME/src/lib/core/files

# build front
# --no-cache to prevent using old core/files
docker build --no-cache -t $FRONT_CONTAINER_NAME $FRONT_CONTAINER_NAME -f $FRONT_CONTAINER_NAME/Dockerfile.build.front --load
docker cp $(docker create --rm $FRONT_CONTAINER_NAME):/app/build/. ./mpp-front/build-out

# build the server for all platforms
docker buildx build --platform $TARGET_PLATFORMS --push --no-cache -t $REPO:$VERSION $FRONT_CONTAINER_NAME -f $FRONT_CONTAINER_NAME/Dockerfile.final

# push to tag and latest
docker tag $REPO:$VERSION $REPO:latest && docker push $REPO:latest

# remove dirt
# docker rmi $(docker images "$CORE_CONTAINER_NAME" -a -q) -f &> /dev/null
# docker rmi $(docker images "$REPO:latest" -a -q) -f &> /dev/null
# rm -rf ./mpp-front/build-out &> /dev/null