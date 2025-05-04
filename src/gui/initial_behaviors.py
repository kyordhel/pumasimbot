# initial_genetics_behaviors.py
'''
@author: Jesus Savage, UNAM-FI, 5-2025
'''
import sys
import math
import os


# Initial Path to be set in the GUI
INITIAL_PATH = '../../data/'
ENVIRONMENT = 'obstacle'
#ENVIRONMENT = 'random_2'
BEHAVIOR = "student"


if len (sys.argv) < 2 :
    print ("\n**********************************************************************************************************")
    print(" ")
    print("                                         P U M A S I M B O T ")
    print(" ")
    print ("Usage: python3 pumasimbot.py num_behavior")
    print ("Where num_behavior:")
    print ("1 = Avoid obstacles and search for light source reactive behavior without memory")
    print ("2 = Avoid obstacles behavior using a FSM")
    print ("3 = Search for a light behavior using a FSM")
    print ("4 = Search for a light source and avoid obstacles bahavior using a FSM")
    print ("5 = Student behavior 1")
    print ("6 = Student behavior 2")
    print ("7 = First Search / Dijkstra Algorithms combined with a search for a light source and avoid obstacles bahavior using a FSM")
    print ("8 = Search for a light source and avoid obstacles bahavior using the rule base system CLIPS")
    print ("9 = Action planning example using CLIPS")
    print ("10 = Other Algorithms\n")
    print ("Example:")
    print ("python3 pumasimbot.py 4")
    print ("\n**********************************************************************************************************")
    sys.exit (1)


#for x in sys.argv:
     #print "Argument: ", x

num_behavior = int(sys.argv[1])

print ("\n**********************************************************************************************************")

if num_behavior == 1:
        num_bh = 1
        print ("*               Testing a reactive behavior with cero order logic                                       *")
elif num_behavior == 2:
	num_bh = 2
	print ("* 		Testing a deterministic FSM to avoid obstacles*")
elif num_behavior == 3:
	num_bh = 3
	print ("* 		Testing a deterministic FSM to follow a light source					 *")
elif num_behavior == 4:
	num_bh = 4
	print ("* 		Testing a deterministic FSM to avoid obstacles and to follow a light source		 *")
elif num_behavior == 5:
	num_bh = 5
	print ("* 		Testing an student FSM								 	 *")
elif num_behavior == 6:
	num_bh = 6
	print ("*               Testing an student FSM           							*")
elif num_behavior == 7:
	num_bh = 7
	print ("*               First Search / Dijkstra Algorithms          						*")
elif num_behavior == 8:
	num_bh = 8
	print ("*               Testing behavior using a rule base system CLIPS                                          *")
elif num_behavior == 9:
	num_bh = 9
	print ("*               Testing action planning example using CLIPS                                              *")
elif num_behavior == 10:
	num_bh = 10
	print ("*               Testing other algorithms                                                                 *\n")
else:
	print ("Behavior does not exist")
	print ("Usage: python GUI_robotics_students.py num_behavior")
	print ("Where num_behavior:")
	print ("1 = Avoid obstacle and search light reactive behavior")
	print ("2 = Avoid obstacle behavior")
	print ("3 = Search light behavior")
	print ("4 = FSM to search light and avoid obstacle bahavior")
	print ("5 = Student behavior 1")
	print ("6 = Student behavior 2")
	print ("7 = First Search / Dijkstra Algorithms")
	print ("8 = Search for a light source and avoid obstacles bahavior using the rule base system CLIPS")
	print ("9 = Action planning example using CLIPS")
	print ("10 = Other Algorithms\n")
	print ("Example:")
	print ("python3 pumasimbot.py 4")
	print ("**********************************************************************************************************\n" )

	sys.exit (1)
	

print ("**********************************************************************************************************\n" )

