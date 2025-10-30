#!/usr/bin/env bash

# CMAKE_FLAGS="-DTCP_CLIPS60_FETCH_FROM_GIT=1 -DTCP_MINIBOT_FETCH_FROM_GIT=1"
CMAKE_FLAGS="-DDEBUG=1 -DTCP_CLIPS60_FETCH_FROM_GIT=1 -DMINIBOT_DRIVER_PATH=~/Develop/minibot/driver -DMINIBOT_TCP_API_PATH=~/Develop/pumasimbot/v4/minibot-tcp-api"

[ -d build ] || mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug ${CMAKE_FLAGS} && make
RET=$?
cd ..
exit ${RET}
