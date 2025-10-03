#!/usr/bin/env bash

# CMAKE_FLAGS="-DTCP_CLIPS60_FETCH_FROM_GIT=1 -DTCP_MINIBOT_FETCH_FROM_GIT=1"
CMAKE_FLAGS="-DTCP_CLIPS60_FETCH_FROM_GIT=1 -DMINIBOT_TCP_API_PATH=~/Develop/pumasimbot/v4/minibot-tcp-api"

[ -d build ] || mkdir -p build
cd build
cmake .. ${CMAKE_FLAGS} && make
cd ..
exit 0
