#!/usr/bin/env python3
import os, os.path
import sys

PUMASIMBOT_ROOT = None
PUMASIMBOT_DATA = None
PUMASIMBOT_BIN  = None
PUMASIMBOT_START_PLANNING  = None


def __find_pumasimbot_root():
	global PUMASIMBOT_ROOT
	here = os.getcwd()
	ps = 'pumasimbot'
	pspos = here.rfind(ps)
	if pspos != -1:
		PUMASIMBOT_ROOT = here[0:pspos + len(ps)]
	elif here.endswith( '/src/gui' ) or here.endswith( '/bin/gui' ):
		PUMASIMBOT_ROOT = here[0:-8]
	else:
		print('Cannot find pumasimbot root path. Bad directory structure.', file=stderr)
		sys.exit(-69)
# end def

def __find_pumasimbot_dirs():
	global PUMASIMBOT_BIN, PUMASIMBOT_DATA, PUMASIMBOT_START_PLANNING
	if PUMASIMBOT_ROOT is None: __find_pumasimbot_root()
	PUMASIMBOT_BIN = os.path.join(PUMASIMBOT_ROOT, 'bin')
	PUMASIMBOT_DATA = os.path.join(PUMASIMBOT_ROOT, 'data')
	PUMASIMBOT_START_PLANNING = os.path.join(PUMASIMBOT_ROOT, 'start_planning.sh')
# end def

__find_pumasimbot_dirs()

if __name__ == '__main__':
	print(f'PUMASIMBOT_ROOT: {PUMASIMBOT_ROOT}')
	print(f'PUMASIMBOT_DATA: {PUMASIMBOT_DATA}')
	print(f'PUMASIMBOT_BIN: {PUMASIMBOT_BIN}')
	print(f'PUMASIMBOT_START_PLANNING: {PUMASIMBOT_START_PLANNING}')
# end if
