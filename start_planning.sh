#!/bin/bash
if [[ $(pwd) == *bin/gui ]]; then
	cd ../..
fi

if [ ! -f bin/tcpCLIPS60/clipsserver ]; then
	if [ ! -d build ]; then
		mkdir -p build;
	fi
	cd build;
	cmake .. -DTCP_CLIPS60_FETCH_FROM_GIT=1 && make;
	cd ..
fi


#Script to start CLIPS
xterm -geometry 80x27+100+100 -hold -e "pwd && cd bin/tcpCLIPS60 && ./clipsserver -d expert_system" &
sleep 0.3
xterm -geometry 160x27+100+480 -hold -e "pwd && cd bin/tcpCLIPS60 && ./clipscontrol -d expert_system" &
sleep 0.1
