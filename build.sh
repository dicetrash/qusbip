#!/bin/bash

echo "1 for debian"
echo "2 for arch"

read -p "what platform would you like to build?: " PLATFORM 

if [ $PLATFORM == "1" ]; then
  docker build -f builds/Dockerfile.debian -t qusbip:deb .
  CONTAINER=${docker create qusbip:dev}
  docker cp ${CONTAINER}:/opt/qusbip.deb qusbip.deb
  docker rm ${CONTAINER}
elif [ $PLATFORM == "2" ]; then
  docker build -f builds/Dockerfile.arch -t qusbip:arch .
  CONTAINER=${docker create qusbip:arch}
  docker cp ${CONTAINER}:/opt/qusbip.tar.gz qusbip.tar.gz
  docker rm ${CONTAINER}
else
  echo "platform not found"
fi
