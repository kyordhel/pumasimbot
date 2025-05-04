#!/usr/bin/env bash

[ -d build ] || mkdir -p build
cd build
cmake .. -DTCP_CLIPS60_FETCH_FROM_GIT=1 && make
# cmake .. -DTCP_CLIPS60_PATH=~/develop/tcpclips60 && make
cd ..
exit 0
