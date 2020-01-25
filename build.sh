#!/bin/bash

echo "1 for debian"
echo "2 for arch"

read -p "what platform would you like to build?: " PLATFORM 

if [ $PLATFORM == "1" ]; then
  docker build -f builds/Dockerfile.debian -t qusbip:deb .
  CONTAINER=$(docker create qusbip:deb)
  docker cp ${CONTAINER}:/opt/qusbip.deb qusbip.deb
  docker rm ${CONTAINER}
elif [ $PLATFORM == "2" ]; then
  docker build -f builds/Dockerfile.arch -t qusbip:arch .
  CONTAINER=$(docker create qusbip:arch)
  docker cp ${CONTAINER}:/opt/qusbip-0_1-1-x86_64.pkg.tar.xz qusbip.pkg.tar.xz
  docker rm ${CONTAINER}
else
  echo "platform not found"
fi
