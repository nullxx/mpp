#!/bin/bash

docker build -t mpp-core -f Dockerfile.build.core .

docker cp $(docker create --rm test):/mpp-cpu/output/. mpp-front/src/lib/core/files
mv mpp-front/src/lib/core/files/output/* mpp-front/src/lib/core/files
rm -rf mpp-front/src/lib/core/files/output

docker rmi $(docker images | grep 'mpp-core')

docker build -t mpp-front mpp-front

echo "Frontend built on mpp-front"
echo "Build finished"