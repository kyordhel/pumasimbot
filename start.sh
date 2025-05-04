#!/bin/bash

if [ ! -f bin/tcpCLIPS60/clipsserver ]; then
	if [ ! -d build ]; then
		mkdir -p build;
	fi
	cd build;
	cmake .. -DTCP_CLIPS60_FETCH_FROM_GIT=1 && make;
	cd ..
fi

#Script to start CLIPS
# xterm -geometry 80x27+100+100 -hold -e "cd bin/tcpCLIPS60 && ./clipsserver" &
# sleep 0.3
# xterm -geometry 160x27+100+480 -hold -e "cd bin/tcpCLIPS60 && ./clipscontrol" &
# sleep 0.1
xterm -geometry 80x27+585+100 -hold -e "cd bin/gui && python3 pumasimbot.py 9"

