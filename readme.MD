# PUMASIMBOT
PUMASIMBOT is a system to test robots behaviors.

To use it please follow these steps:
1. Clone this repository
2. Enter the directory of the just-cloned repository
3. Execute build.sh
4. Execute start.sh
5. Once you see the simulator, click on all three plot buttons (Map, Robot Behavior, Topological) and then on `Example Action Planner`

Or, in bash:
```bash
git clone https://github.com/kyordhel/pumasimbot.git
cd pumasimbot
./build.sh && ./start.sh
```

Enjoy!

# Prerrequisites
PUMASIMBOT requires just
the GNU C Compiler,
cmake,
boost,
python3 with tkinter,
and a working copy of [tcpCLIPS60](https://github.com/kyordhel/tcpclips60)
that will be automagically downloaded by CMake, although you can override this behavior as explained in [Build](#build).

## Prerrequisites' installation under ubuntu/debian
```bash
sudo apt install build-essential cmake libboost-all-dev \
libncurses-dev python3 python3-tk python3-numpy
```

# Build
For convenience, and because most students lack an attention span that lasts longer than a tik-tok video or are incapable of glancing over a document longer than a tweet, the `build.sh` script was provided.
But that's not the best way to learn to build cmake projects.
Thus we provide a more-in-detail explanation on the process below.

First, create the `build` directory if it doesn't already exist, and get in there:
```bash
mkdir -p build
cd build
```

The next step would be invoking CMake.
However, PUMASIMBOT requires a working copy of [tcpCLIPS60](https://github.com/kyordhel/tcpclips60) to operate, and therefore we must either
a) tell CMake where to find `tcpclips60` in our system,
XOR
b) tell CMake to download`tcpclips60` from the Internet.

If you have previously downloaded and compiled, execute the following commands:
```bash
cmake .. -DTCP_CLIPS60_PATH=path/to/tcpclips60
```

Or, if if you want CMake to download and build it for you, execute instead:
```bash
cmake .. -DTCP_CLIPS60_FETCH_FROM_GIT=1
```

If everything goes well (and there is no reason why it should not), the final step would be to compile everything by invoking `make`

```bash
make
```

**Note:** All binaries will be placed inside the `bin` directory.

# Testing
Testing PUMASIMBOT is rather straightforward.
Locate yourself into the `gui` directory inside `bin` and execute the simulator:
```bash
cd bin/gui
python3 pumasimbot.py
```

And that's about it.
