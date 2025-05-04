# pumasimbot.py 
#from MobileRobotSimulator import *
'''
@author: Jesus Savage, UNAM-FI, 5-2025
'''
import os
import tkinter as tk
from tkinter import *
import math
import random
from random import randrange, uniform
import time
import os
import numpy as np
from initial_behaviors import *
import time
 
#set softtabstop=0 noexpandtab 


#---------------------------------------------------------------------------------------
#	Global Variables

use_gui = True
gui = None
gui_planner = None
gui_example = None
debug = False
count = 0
DIM_CANVAS_X = 400
DIM_CANVAS_Y = 400
dim_x = 10.0
dim_y = 10.0
radio_robot = .03
pose_x = 4.0
pose_y = 3.0
pose_tetha = 0.0
mouse_1_x = 0.0
mouse_1_y = 0.0
mouse_2_x = 0.0
mouse_2_y = 0.0
mouse_3_x = 0.0
mouse_3_y = 0.0
num_pol = 0
polygons = []
flg_mov = 1
flg_sensor = 1
delay = .05
PATH = 'path'
File_Name = 'file_name'
File_Name_robot = 'file_name'
angle_robot = 0.0
sensor = "laser"
num_sensors = 2
flg_noise = 1
start_angle = -0.1
range_angle = 0.2
flg_execute = 1
robot_command = "../motion_planner/GoTo_State_Machine"
flg_plt = 1
vq = 0
#size_vq = 4
#pr_out = 0
flg_unk = 0
#number_unk = 0
#previous_num = 0
#previous_data = [0] * 2
number_steps_total = 300
varShowNodes   = False
STOP = 0
flg_line = 0
largest_value = 0.0
flg_start_clips = 1
original_value = 0.0

#-------------------------------------------------------------------------------------------
#	TK Definitions

planner = tk.Tk()
planner.wm_title('PUMAS ROBOT SIMULATOR')
C = tk.Canvas(planner, bg="green", height=DIM_CANVAS_X, width=DIM_CANVAS_Y)


class PLANNER(object):
   
	global robot_angle

	def __init__(self):

		global C
		global DIM_CANVAS_X 
		global DIM_CANVAS_Y
		global dim_x
		global dim_y
		global x
		global y
		global radio_robot
		global pose_x
		global pose_y
		global pose_tetha
		global robot_angle


 
	def initial():
		global PATH
		global File_Name
		global File_Name_robot
		global flg_mov
		global flg_sensor
		global delay
		global flg_plt
		global new_generation
		global flg_noise
		global BEHAVIOR
		global size_vq_ext
		global bh
		global advance_robot

		topLevelWindow = Tk()
		topLevelWindow.wm_title('GUI_ROBOTS')



#---------------------------------------------- Buttons Fields --------------------------------------------------------------------------------------------

		# Plot Robot button
		#RobotButton = tk.Button(topLevelWindow, width = 20, text = 'Plot Robot Behavior', bg = 'green', activebackground = 'green', command = togglePlotRobot)
		countRobot = 0
		# Plot Sensor button
		#ExecuteButton = tk.Button(topLevelWindow, width = 20, text = 'Execute Robot Command ', bg = 'green', activebackground = 'green', command = togglePlotExecute)
		countExecute = 0
		countMap = 0
		# Plot Path button
		#PathButton = tk.Button(topLevelWindow, width = 20, text = 'Plot Path', bg = 'green', activebackground = 'green', command = togglePlotPath)
		countPath = 0
      		# Path files entry 
		label_path = tk.Label(topLevelWindow,text =  'Path')
		path = tk.Entry(topLevelWindow, width = 30, foreground='white',background='black')
		##self.path.insert ( 0, '/home/biorobotica/data/data_15/' )
		path.insert ( 0, INITIAL_PATH )
		PATH = path.get()
      		# World's File entry 
		label_file = tk.Label(topLevelWindow,text =  'World description')
		file = tk.Entry(topLevelWindow, width = 30, foreground='white',background='black')
		##self.file.insert ( 0, 'room' )
		##self.file.insert ( 0, 'random' )
		file.insert ( 0, ENVIRONMENT )
		File_Name = file.get()
		#print ('file ',File_Name)
      		# Robot's File entry 
		label_file_robot = tk.Label(topLevelWindow,text =  'Robot Behavior File ')
		file_robot = tk.Entry(topLevelWindow, width = 30, foreground='white',background='black')
		##self.file_robot.insert ( 0, 'random' )
		file_robot.insert ( 0, ENVIRONMENT)
		File_Name_robot = file_robot.get()

		

		# Check button movement
		var_mov = IntVar()
		def command_mov():
                    if var_mov.get() == 1:
                        Movement.deselect()
                        #print("Checkbutton mov is deselected")
                        var_mov.set(0)
                    else:
                        Movement.select()
                        #print("Checkbutton mov is selected")
                        var_mov.set(1)
                    #print ("Checkbutton variable var_mov is ", var_mov.get())

		Movement = tk.Checkbutton(topLevelWindow, text="Show robot movements", variable= var_mov, onvalue=1, offvalue=0,command=command_mov)
		Movement.deselect()
		var_mov.set(0)


		# Check button sensor
		var_sensor = IntVar()
		def command_sensor():
			if var_sensor.get() == 0:
                                sensor.select()
                                var_sensor.set(1)
                                #print("Checkbutton sensor is selected")
			else:
                                sensor.deselect()
                                var_sensor.set(0)
                                #print("Checkbutton sensor is deselected")
			#print ("Checkbutton variable var_sensor is ", var_sensor.get())

		sensor = tk.Checkbutton(topLevelWindow, text="Show sensors", variable= var_sensor, onvalue=1, offvalue=0, command=command_sensor)
		sensor.deselect()
		var_sensor.set(0)


		# Check button sensor line
		var_line = IntVar()
		def command_sensor_line():
			if var_line.get() == 0:
                                sensor_line.select()
                                var_line.set(1)
                                #print("Checkbutton var_line is selected")
			else:
                                sensor_line.deselect()
                                var_line.set(0)
                                #print("Checkbutton var_line is deselected")
			#print ("Checkbutton variable var_line is ", var_line.get())

		sensor_line = tk.Checkbutton(topLevelWindow, text="Show sensors lines", variable= var_line, onvalue=1, offvalue=0, command=command_sensor_line)
		sensor_line.deselect()
		var_line.set(0)






	        # Check button add_noise
		add_noise = IntVar()
		def command_add_noise():
			if add_noise.get() == 0:
				noise.select()
				add_noise.set(1)
				#print("Checkbutton noise is selected")
			else:
				noise.deselect()
				add_noise.set(0)
				#print("Checkbutton noise is deselected")
			#print ("Checkbutton variable add_noise is ", add_noise.get())

		noise = tk.Checkbutton(topLevelWindow, text="Add noise", variable= add_noise, onvalue=1, offvalue=0,command=command_add_noise)
		noise.deselect()
		add_noise.set(0)

	        # Check button add_noise
		method_search = IntVar()
		def command_method():
			if method_search.get() == 1:
				method.select()
				method_search.set(2)
				#print("Checkbutton method is selected")
			else:
				method.deselect()
				method_search.set(1)
				#print("Checkbutton method is deselected")
			#print ("Checkbutton variable method is ", method_search.get())

		method = tk.Checkbutton(topLevelWindow, text="Topological Search First/Dijkstra", variable= method_search, onvalue=2, offvalue=1,command=command_method)
		method.deselect()
		method_search.set(1)

		
		
                # Plot Topological map button
		def print_topological_map_lines(): # It plots  the topological map of the current map  

                	#self.clear_topological_map();
			varShowNodes = True

                	#self.w.delete(self.nodes_image)        
			nodes_coords = []
			#image = Image.new('RGBA', (DIM_CANVAS_X,DIM_CANVAS_Y))
			#draw = ImageDraw.Draw(image)
			PATH = path.get()
			File_Name = file.get()
			#print ('Path ',PATH) 
			#print ('File ',File_Name)
			FILE = PATH + File_Name + '.top'
			map_file = open(FILE, 'r')
			#map_file = open(self.rospack.get_path('simulator')+'/src/data/'+self.entryFile.get()+'/'+self.entryFile.get()+'.top','r')                  #Open file
			lines = map_file.readlines()                          #Split the file in lines

			for line in lines:        #To read line by line
				words = line.split()       #To separate  words 
				#print len(words)
				#for i in range(0, len(words)):
				#print (words[i])
				if words:          #To avoid empty lines                                                 
					if words[0] == "(":        #To avoid coments
						if words[1] == "num":                     #To get world dimensions
							numNode = float (words[3])
						elif words[1] == "node":                                  #to get polygons vertexi
							numNode = words[2]
							nodeXm = float (words[3]) * DIM_CANVAS_X / dim_x
							nodeYm = (dim_y - float (words[4])) * DIM_CANVAS_Y / dim_y
							#print ('word[3] ' + words[3] + ' words[4] ' + words[4])
							#print ('Xm ' + str(nodeXm) + ' Ym ' + str(nodeYm))
							C.update()
							id = C.create_rectangle(nodeXm, nodeYm, nodeXm+2, nodeYm+2, fill= "darkblue")
							nodes_coords.append([nodeXm,nodeYm])
						elif words[1] == "connection":                            #to get polygons vertex
							c1 = int(words[2])
							c2 = int(words[3])
							#print ('Node C1 ' + str(c1) + ' C2 ' + str(c2))
							x1 = nodes_coords[c1][0]
							y1 = nodes_coords[c1][1]
							x2 = nodes_coords[c2][0]
							y2 = nodes_coords[c2][1]
							#print ('X1 ' + str(x1) + ' Y1 ' + str(y1))
							#print ('X2 ' + str(x2) + ' Y2 ' + str(y2))
							line = C.create_line(x1,y1,x2,y2,fill="darkblue", arrow="last")
							C.update_idletasks()
							time.sleep(0.001) # 0.001 delay seconds to see the plot of the lines

			#print ('dim_x ' + str(dim_x) + ' dim_y ' + str(dim_y))

			map_file.close()


		def plot_line(x1,y1,x2,y2,color,flg):
			global C
			global flg_line
			global largest_value

			#print ("plot_line ")
			#print ("x1 ",x1," y1 ",y1)
			X1 = ( DIM_CANVAS_X * x1 ) / dim_x
			Y1 = DIM_CANVAS_Y - ( DIM_CANVAS_Y * y1 ) / dim_y
			#print ("X1 ",X1," Y1 ",Y1)

			#print ("x2 ",x2," y2 ",y2)
			X2 = ( DIM_CANVAS_X * x2 ) / dim_x
			Y2 = DIM_CANVAS_Y - ( DIM_CANVAS_Y * y2 ) / dim_y
			#print ("X2 ",X2," Y2 ",Y2)

			#print("flg ",flg,"flg_line ",flg_line)
			if flg == 1:
				line = C.create_line(X1,Y1,X2,Y2,fill=color, arrow="last")
			else:
				if flg_line == 1:
					line = C.create_line(X1,Y1,X2,Y2,fill=color)
					id = C.create_rectangle(X2,Y2,X2+1,Y2+1, fill= "white", outline="yellow")
				difx = x2-x1
				dify = y2-y1
				mag = math.sqrt(pow(difx,2)+pow(dify,2))
				#print("line x1 ",x1," y1 ",y1," x2 ",x2," y2 ", y2," mag ",mag)
				if mag < 0.75*largest_value:
					id = C.create_rectangle(X2,Y2,X2+1,Y2+1, fill= "white", outline="white")





		def plot_robot():
			global C
			global pose_x
			global pose_y
			global radio_robot
			global pose_tetha


			CNT=5.0
			#print ("plot robot pose_tetha ",pose_tetha)
			X = ( DIM_CANVAS_X * pose_x ) / dim_x
			Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * pose_y ) / dim_y
			ROBOT_radio = ( DIM_CANVAS_X * radio_robot ) / dim_x
			X1 = X - ROBOT_radio/2
			Y1 = Y - ROBOT_radio/2
			X2 = X + ROBOT_radio/2
			Y2 = Y + ROBOT_radio/2
			#print ("X1 ", X1, " Y1 ", Y1)
			#print ("X2 ", X2, " Y2 ", Y2i)
			#oval = C.create_oval(X1,Y1,X2,Y2, outline="black",fill="red", width=1)
			oval = C.create_oval(X1,Y1,X2,Y2, outline="black",fill="black", width=1)
			#oval = C.create_oval(X1,Y1,X2,Y2, outline="green",fill="green", width=1)
			#X1 = X
			#Y1 = Y
			#Y2 = Y - CNT*ROBOT_radio*math.sin(pose_tetha)
			#print ("sen ", pose_tetha, " = ",math.sin(pose_tetha))
			#line = C.create_line(X1,Y1,X2,Y2,fill="black", arrow="last")

			x1 = pose_x
			y1 = pose_y
			angle_robot = pose_tetha
			tetha = angle_robot + start_angle
			x2 = x1 + (dim_x/25)*math.cos(angle_robot)
			y2 = y1 + (dim_y/25)*math.sin(angle_robot)
			plot_line(x1,y1,x2,y2,"black",1)
			#plot_line(x1,y1,x2,y2,"green",1)
			
			mouse_3_x = pose_x
			mouse_3_y = pose_y

	

		def print_topological_map(): # It plots  the topological map of the current map  and  show  "please wait" message
			C.update()
			buttonPlotTopological['bg'] = 'red'
			buttonPlotTopological['activebackground'] = 'red'
			togglePlotMap()
			print_topological_map_lines()
			buttonPlotTopological['bg'] = 'green'
			buttonPlotTopological['activebackground'] = 'green'
			#w.delete(wait)
			#w.delete(wait_bg)
			plot_robot()

		buttonPlotTopological= Button(topLevelWindow ,width = 20, text = "Plot Topological", bg = 'green', activebackground = 'green',command = print_topological_map )


		def stop_execution(): # It exits the system  
			global STOP

			C.update()
			buttonStop['bg'] = 'red'
			buttonStop['activebackground'] = 'red'
			STOP = 1
			buttonStop['bg'] = 'green'
			buttonStop['activebackground'] = 'green'
			exit()

		buttonStop = Button(topLevelWindow ,width = 20, text = "EXIT", bg = 'green', activebackground = 'green',command = stop_execution )



		def show_help(): # It shows information on how to use the simulator  

			C.update()
			buttonHelp['bg'] = 'red'
			buttonHelp['activebackground'] = 'red'
		
			print(" ")
			print(" 					P U M A S I M B O T ")
			print(" ")
			print("In the PUMAS ROBOT SIMULATOR window select the robot’s origin with the mouse’s left button")
			print("and select the robot’s destination with the mouse’s right button.")
			print("Then, the robot will execute the behavior selected in the field: Behavior Selection")
			print ("To change the robot behavior in the field Behavior Selection put:")
			print ("	1 = Behavior to avoid obstacles and look for light source without memory")
			print ("	2 = Behavior to avoid obstacles with memory (FSM)")
			print ("	3 = Behavior to look for a light source with memory (FSM)")
			print ("	4 = Behavior to look for light source and avoid obstacles with memory (FSM)")
			print ("	5 = Student behavior 1")
			print ("	6 = Student behavior 2")
			print ("	7 = Behavior to look for a light source and avoid obstacles with memory (FSM), using a topological map with First Search or Dijkstra Algorithms")
			print ("	8 = Search for a light source and avoid obstacles bahavior using the rule base system CLIPS")
			print ("	9 = Action planning example using CLIPS")
			print ("	10 = Other Algorithms\n")

			#print(" ")
			print("The enviroment can be changed in the field: World description")
			print("The behavior file where the robot behavior is saved, after execution, can be change in the field: Robot Behavior File")
			print("This file with the robot behavior can ploted by selecting Plot Robot Behavior")

			buttonHelp['bg'] = 'green'
			buttonHelp['activebackground'] = 'green'

		buttonHelp = Button(topLevelWindow ,width = 20, text = "HELP", bg = 'green', activebackground = 'green',command = show_help )
		show_help()

		def Start_CLIPS(): 
			if flg_start_clips == 1:
				#command = "~/tcpclips60-master/start.sh"
				#command = "~/develop/tcpclips60/start.sh"
				command = "../../start_planning.sh"
				print ("Start CLIPS ",command)
				status = os.system(command)
				print("status ",status)


		buttonClips = Button(topLevelWindow ,width = 20, text = "CLIPS", bg = 'green', activebackground = 'green',command = Start_CLIPS )
		

		def Start_Example(): 
			global mouse_1_x
			global mouse_1_y
			global mouse_3_x
			global mouse_3_y

		    	
			num_bh = 9
			selection.delete(0, END)
			selection.insert(0,str(num_bh))
			selection_value = selection.get()
			BEHAVIOR = selection.get()
			number_steps_total = 15000
			number_steps.delete(0, END)
			number_steps.insert ( 0, str(number_steps_total) )
			number_steps_value = number_steps.get()
			file.delete(0, END)
			ENVIRONMENT = "final"
			file.insert ( 0, ENVIRONMENT )
			File_Name = file.get()
			file_robot.delete(0, END)
			file_robot.insert ( 0, ENVIRONMENT)
			File_Name_robot = file_robot.get()

			togglePlotMap()
			print_topological_map_lines()

			mouse_1_x = 0.25
			mouse_1_y = 0.25
			angle_robot= 0

			mouse_3_x = 0.50
			mouse_3_y = 0.50
			flg_execute = 1

			if flg_execute == 1:
				if var_mov.get() == 0:
					C.update_idletasks()

					flg_plt = 1
					BEHAVIOR = selection.get()
					togglePlotExecute(0,BEHAVIOR)

					PATH = path.get()
					#print ('Evaluate Robot PATH ',PATH)
					File_Name = file.get()
					FILE = PATH + File_Name + '.raw'
					#print ('Evaluate Robot File_Name ',FILE)

					File_Constants = PATH + 'Constants.txt'
					partial_evaluation = readResultFile(FILE,File_Constants,0)
					print ('Evaluation ',partial_evaluation)
					evaluation_ind.delete(0, END)
					evaluation_ind.insert ( 0, str(partial_evaluation))

			print_topological_map_lines()



		buttonExample = Button(topLevelWindow ,width = 20, text = "Example Action Planner", bg = 'green', activebackground = 'green',command = Start_Example )



		def plot_polygon(num, data):
			global C

			XY= data
			#print ("plot_polygon ",int(num))
			for i in range(0, int(num)):
				j=i*2

				#print ("data j ",j," x",data[j]," y ",data[j+1])
				X = ( DIM_CANVAS_X * data[j] ) / dim_x
				Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * data[j+1] ) / dim_y
				XY[j]=X;
				XY[j+1]=Y;
				#print ("j ",j," X",XY[j]," Y ",XY[j+1])

			polygon = C.create_polygon(XY, outline='black', fill='red', width=1)
			return polygon



		def read_file_map(flg):
			global pose_x
			global pose_y
			global pose_tetha
			global C
			global num_pol
			global polygons
			global PATH
			global dim_x
			global dim_y
			#global File_Name


			id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "green")

			PATH = path.get()
			File_Name = file.get()
	
			#print ('Path ',PATH) 
			#print ('File ',File_Name)

			FILE = PATH + File_Name + '.wrl'

			filer = open(FILE, 'r')

			for line in filer:
				#print ('line',line)
				words = line.split()
				data = words
				#print (len(words))
				#for i in range(0, len(words)):
				#print (words[i])
				if len(words) > 1:
					if words[0] == ";(":
						j=0
					elif words[1] == "dimensions":
						dim_x = float(words[3])
						dim_y = float(words[4])
						#print ('dim_x ',dim_x, 'dim_y ',dim_y)
					elif words[1] == "polygon":
						j=0
						data = [0] * (len(words) - 5)
						for i in range(4, len(words) -1):
							#print(j,words[i])
							data[j] = float(words[i])
							j=j+1

						#j=j+1
						num = j / 2
						if flg == 1:
							polygons.append(plot_polygon(num,data))
							num_pol = num_pol + 1
						else:
							plot_polygon(num,data)
							
			filer.close()


		def plot_oval_green1(x,y):
			global C
			global radio_robot


			CNT=5.0
			#print ("plot robot pose_tetha ",pose_tetha)
			X = ( DIM_CANVAS_X * x ) / dim_x
			Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * y ) / dim_y
			ROBOT_radio = ( DIM_CANVAS_X * radio_robot ) / dim_x
			#X1 = X - ROBOT_radio/2
			#Y1 = Y - ROBOT_radio/2
			#X2 = X + ROBOT_radio/2
			#Y2 = Y + ROBOT_radio/2
			X1 = X - 1.5*ROBOT_radio
			Y1 = Y - 1.5*ROBOT_radio
			X2 = X + 1.5*ROBOT_radio
			Y2 = Y + 1.5*ROBOT_radio
			#print ("X1 ", X1, " Y1 ", Y1)
			#print ("X2 ", X2, " Y2 ", Y2)
			oval = C.create_oval(X1,Y1,X2,Y2, outline="green",fill="green", width=1)



		def plot_oval(x,y):
			global C
			global radio_robot


			CNT=5.0
			#print ("plot robot pose_tetha ",pose_tetha)
			X = ( DIM_CANVAS_X * x ) / dim_x
			Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * y ) / dim_y
			ROBOT_radio = ( DIM_CANVAS_X * radio_robot ) / dim_x
			#X1 = X - ROBOT_radio/2
			#Y1 = Y - ROBOT_radio/2
			#X2 = X + ROBOT_radio/2
			#Y2 = Y + ROBOT_radio/2
			X1 = X - 1.5*ROBOT_radio
			Y1 = Y - 1.5*ROBOT_radio
			X2 = X + 1.5*ROBOT_radio
			Y2 = Y + 1.5*ROBOT_radio
			#print ("X1 ", X1, " Y1 ", Y1)
			#print ("X2 ", X2, " Y2 ", Y2)
			oval = C.create_oval(X1,Y1,X2,Y2, outline="black",fill="yellow", width=1)



		def plot_oval_green(x,y):
			global C
			global radio_robot


			CNT=5.0
			#print ("plot robot pose_tetha ",pose_tetha)
			X = ( DIM_CANVAS_X * x ) / dim_x
			Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * y ) / dim_y
			ROBOT_radio = ( DIM_CANVAS_X * radio_robot ) / dim_x
			#X1 = X - ROBOT_radio/2
			#Y1 = Y - ROBOT_radio/2
			#X2 = X + ROBOT_radio/2
			#Y2 = Y + ROBOT_radio/2
			X1 = X - 1.5*ROBOT_radio
			Y1 = Y - 1.5*ROBOT_radio
			X2 = X + 1.5*ROBOT_radio
			Y2 = Y + 1.5*ROBOT_radio
			#print ("X1 ", X1, " Y1 ", Y1)
			#print ("X2 ", X2, " Y2 ", Y2)
			oval = C.create_oval(X1,Y1,X2,Y2, outline="green",fill="green", width=1)



		def read_file(flg):
			global pose_x
			global pose_y
			global pose_tetha
			global C
			global num_pol
			global polygons
			global flg_mov
			global flg_sensor
			global delay
			global dim_x
			global dim_y
			global angle_robot
			global radio_robot
			global number_steps_total
			global flg_unk
			global previous_num
			global previous_data
			global number_unk
			global STOP
			global flg_line
			global largest_value

			number_unk = int(movable.get())
			PATH = path.get()
			File_Name_robot = file_robot.get()
			FILE = PATH + File_Name_robot + '.raw'
			file = open(FILE, 'r')

			C.update_idletasks() # it updates the ide data
			delay = 0
			flg_mov = var_mov.get()
			flg_sensor = var_sensor.get()
			flg_line = var_line.get()
			largest_value = float(largest.get())
			flg_plot_vq = 0
			number_steps_value = number_steps.get()
			number_steps_total = float(number_steps_value)
			num_steps = 1
			cnt_unk = 1
			flg_destination = 0

			for line in file:

				if STOP == 1:
					file.close()
					number_steps.delete(0, END)
					number_steps.insert(0,number_steps_value)
					STOP = 0
					return


				#print ('result line ',line)
				words = line.split()
				data = words
				#print (len(words))
				#for i in range(0, len(words)):
						#print ('words ', i,words[i])
				if len(words) > 1:
					if words[0] == ";(":
						j=0
					elif words[1] == "polygon":
						j=0
						data = [0] * (len(words) - 5)
						for i in range(4, len(words) -1):
							#print (words[i])
							data[j] = float(words[i])
							j=j+1

						j=j+1
						num = j / 2
						if flg == 1:
							polygons.append(plot_polygon(num,data))
							num_pol = num_pol + 1
						else:
							plot_polygon(num,data)

					elif words[1] == "dimensions":
                                        	dim_x = float(words[3])
                                        	dim_y = float(words[4])
                                        	#print ('dim_x ',dim_x, 'dim_y ',dim_y)

					elif words[1] == "radio_robot":
						radio_robot = float(words[2])
						#print ('radio robot ',radio_robot)


					elif words[1] == "delete":
						for i in range(0,num_pol):
							C.delete(polygons[i])

					elif words[1] == "destination":
						x = float(words[2])
						y = float(words[3])
						dest_x = x
						dest_y = y
						plot_oval(x,y)
						if flg_destination == 0:
							x_previous=x
							y_previous=y
							flg_destination=1
						else:
							plot_oval_green(x_previous,y_previous)
							x_previous=x
							y_previous=y
							time.sleep(0.1) # 0.1 delay seconds to see the plot of the destination
					elif words[1] == "object": 
							#print ('object ',words[2])
							numNode = words[2]
							nodeXm = float (words[3]) * DIM_CANVAS_X / dim_x
							nodeYm = (dim_y - float (words[4])) * DIM_CANVAS_Y / dim_y
							#print ('word[3] ' + words[3] + ' words[4] ' + words[4])
							#print ('Xm ' + str(nodeXm) + ' Ym ' + str(nodeYm))
							C.update()
							id = C.create_rectangle(nodeXm, nodeYm, nodeXm+8, nodeYm+8, fill= "darkblue")
							time.sleep(0.2) # 0.1 delay seconds to see the plot of the destination
					elif words[1] == "new_object": 
							#print ('object ',words[2])
							numNode = words[2]
							nodeXm = float (words[3]) * DIM_CANVAS_X / dim_x
							nodeYm = (dim_y - float (words[4])) * DIM_CANVAS_Y / dim_y
							#print ('word[3] ' + words[3] + ' words[4] ' + words[4])
							#print ('Xm ' + str(nodeXm) + ' Ym ' + str(nodeYm))
							C.update()
							x = random.randint(10,14)
							y = random.randint(10,14)
							id = C.create_rectangle(nodeXm, nodeYm, nodeXm+x, nodeYm+y, fill= "darkblue",outline="white")
							time.sleep(0.1) # 0.1 delay seconds to see the plot of the destination
					elif words[1] == "clean": 
							togglePlotMap()
						
					elif words[1] == "erase": 
							#print ('object ',words[2])
							numNode = words[2]
							nodeXm = float (words[3]) * DIM_CANVAS_X / dim_x
							nodeYm = (dim_y - float (words[4])) * DIM_CANVAS_Y / dim_y
							#print ('word[3] ' + words[3] + ' words[4] ' + words[4])
							#print ('Xm ' + str(nodeXm) + ' Ym ' + str(nodeYm))
							C.update()
							id = C.create_rectangle(nodeXm, nodeYm, nodeXm+8, nodeYm+8, fill= "green", outline="green")



					elif words[1] == "connection":                                  #to get polygons vertex
						X1 = float (words[2]) * DIM_CANVAS_X / dim_x
						Y1 = (dim_y - float (words[3])) * DIM_CANVAS_Y / dim_y
						#print ('connection x1 ',X1, 'y1 ',Y1)
						X2 = float (words[4]) * DIM_CANVAS_X / dim_x
						Y2 = (dim_y - float (words[5])) * DIM_CANVAS_Y / dim_y
						#print ('connection x2 ',X2, 'y2 ',Y2)
						C.update()
						#id = C.create_rectangle(X1, Y1, X1+2, Y1+2, fill= "darkblue")
						id = C.create_rectangle(X2, Y2, X2+2, Y2+2, fill= "darkblue")
						#line1 = C.create_line(X1,Y1,X2,Y2,fill="green", arrow="last")
						line1 = C.create_line(X1,Y1,X2,Y2,fill="darkblue", arrow="last")
						#line(X1,Y1,X2,Y2,fill="darkblue", arrow="last")
						C.update_idletasks()
						time.sleep(0.001) # 0.001 delay seconds to see the plot of the lines



					elif words[1] == "unknown":
						flg_unk = 1
						j=0
						data = [0] * (len(words) - 3)
						for i in range(2, len(words) -1):
							#print (words[i])
							data[j] = float(words[i])
							j=j+1

						j=j+1
						num = j / 2
						#value_s = input("Please enter a string:\n")
						if cnt_unk > number_unk:
							if flg_mov == 1:
								#print "clean screen"
								#value_s = input("Please enter a string:\n")
								id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "green")
								read_file_map(0)
							cnt_unk = 1

						#print ("cnt_unk ",str(cnt_unk)," number_unk ",str(number_unk))
						plot_polygon(num,data)
						cnt_unk = cnt_unk +1


					elif words[1] == "robot":
						pose_x = float(words[3])
						pose_y = float(words[4])
						pose_tetha = float(words[5])
						angle_robot = pose_tetha
						str_angle = ("%3.4f" % angle_robot).strip()
						robot_angle.delete(0, END)
						robot_angle.insert(0,str_angle)
						robot_posex.delete(0, END)
						robot_posex.insert (0, words[3] )
						robot_posey.delete(0, END)
						robot_posey.insert (0, words[4] )
						str_num_steps = ("%4d" % num_steps).strip()
						number_steps.delete(0, END)
						number_steps.insert(0,str_num_steps)
						num_steps = num_steps + 1

						if flg_mov == 1:
							if flg_unk == 0:
								id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "green")
								read_file_map(0)

						plot_robot()
						plot_oval(dest_x,dest_y)
						if var_mov.get() == 0:
							C.update_idletasks()
						else:
							if flg_sensor == 0:
								C.update_idletasks()

						if flg_mov == 1:
							time.sleep(delay/2.0) # delays seconds

					elif words[1] == "sensor":
						if flg_plot_vq == 0:
							if words[2] == "laser":
								if flg_sensor == 1:
									if flg_mov == 1:
										if flg_unk == 0:
											id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "green")
											read_file_map(0)

									plot_robot()
									plot_oval(dest_x,dest_y)
									#if flg_unk == 1:
									#plot_polygon(previous_num,previous_data)

									num = int(words[3])

									range_angle = float(words[4])
									start_angle = float(words[5])
									if num == 1:
										delta_angle = range_angle
									else:
										delta_angle = range_angle / (num - 1)
									#print ("num ", num, "Range Measurments ", range_angle)
									#print ("Start angle ", start_angle, " Delta Angle ", delta_angle)

									x1 = pose_x
									y1 = pose_y
									tetha = angle_robot + start_angle
									x2 = x1 + (dim_x/10)*math.cos(angle_robot)
									y2 = y1 + (dim_y/10)*math.sin(angle_robot)
									#plot_line(x1,y1,x2,y2,"red",1)
									#print ("sensor pose tetha ",pose_tetha)
									#print ("sensor angle_robot ",angle_robot)
									#print ("Origen Tetha ", tetha)
									#print ("sen ", tetha, " = ",math.sin(tetha))

									data = [0] * (len(words) - 5)
									j=0
									for i in range(6, len(words) -1):
										#print (words[i])
										#print ("Tetha ", j," ",tetha)
										data[j] = float(words[i])
										x2 = x1 + data[j]*math.cos(tetha)
										y2 = y1 + data[j]*math.sin(tetha)
										plot_line(x1,y1,x2,y2,"blue",0)
										#plot_line(x1,y1,x2,y2,"black",0)
										j=j+1
										tetha = tetha + delta_angle
										#print ("sen ", tetha, " = ",math.sin(tetha))

									C.update_idletasks()
									if flg_mov == 1:
										time.sleep(delay/2.0) # delays seconds

						if flg_plot_vq == 1:
							if words[2] == "vq_laser":
								if flg_sensor == 1:
									if flg_mov == 1:
										if flg_unk == 0:
											id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "green")
											read_file_map(0)

									plot_robot()
									plot_oval(dest_x,dest_y)

									num = int(words[3])
									range_angle = float(words[4])
									start_angle = float(words[5])
									if num == 1:
										delta_angle = range_angle
									else:
										delta_angle = range_angle / (num - 1)
									#print ("num ", num, "Range Measurments ", range_angle)
									#print ("Start angle ", start_angle, " Delta Angle ", delta_angle)

									x1 = pose_x
									y1 = pose_y
									tetha = angle_robot + start_angle
									x2 = x1 + (dim_x/10)*math.cos(angle_robot)
									y2 = y1 + (dim_y/10)*math.sin(angle_robot)
									#plot_line(x1,y1,x2,y2,"red",1)
									#print ("sensor pose tetha ",pose_tetha)
									#print ("sensor angle_robot ",angle_robot)
									#print ("Origen Tetha ", tetha)
									#print ("sen ", tetha, " = ",math.sin(tetha))

									data = [0] * (len(words) - 5)
									j=0
									for i in range(6, len(words) -1):
										#print (words[i])
										#print ("Tetha ", j," ",tetha)
										data[j] = float(words[i])
										x2 = x1 + data[j]*math.cos(tetha)
										y2 = y1 + data[j]*math.sin(tetha)
										#plot_line(x1,y1,x2,y2,"black",0)
										plot_line(x1,y1,x2,y2,"yellow",0)
										#plot_line(x1,y1,x2,y2,"green",0)
										j=j+1
										tetha = tetha + delta_angle
										#print ("sen ", tetha, " = ",math.sin(tetha))


									C.update_idletasks()
									if flg_mov == 1:
										time.sleep(delay/2.0) # delays seconds





			file.close()
			number_steps.delete(0, END)
			number_steps.insert(0,number_steps_value)



		# Plot Map button
		def togglePlotMap():
                
			global PATH
			global File_Name
                
			MapButton['bg'] = 'red'
			MapButton['activebackground'] = 'red'
			C.update_idletasks()

			PATH = path.get()
			#print ('Plot Map PATH ',PATH)
			#File_Name = file.get()
			#print ('Toggle file ',File_Name)
			#print ('Plot Map File_Name ',File_Name)
			flg_mov = var_mov.get()
			#print ('Plot Robot flg_mov',flg_mov)
			flg_sensor = var_sensor.get()
			#print ('Plot Robot flg_sensor ',flg_sensor)
                
			read_file_map(0)
			MapButton['bg'] = 'green'
			MapButton['activebackground'] = 'green'
			str_angle = "0.00000"
			robot_angle.delete(0, END)
			robot_angle.insert(0,str_angle)

		# Plot Map button
		MapButton = tk.Button(topLevelWindow, width = 20, text = 'Plot Map', bg = 'green', activebackground = 'green', command = togglePlotMap)


		def togglePlotRobot():
			global pose_x
			global pose_y
			global pose_tetha
			global PATH
			global File_Name
			global File_Name_robot
			global flg_mov
			global flg_sensor
			global delay

			RobotButton['bg'] = 'red'
			RobotButton['activebackground'] = 'red'

			# uniform gives you a floating-point value
			#pose_x = uniform(0, dim_x)
			#pose_y = uniform(0, dim_y)
			#pose_tetha = uniform(0, 2*3.1416)
			#plot_robot()

			C.update_idletasks()

			PATH = path.get()
			#print ('Plot Robot PATH ',PATH)

			File_Name = file.get()
			#print ('Plot Robot File_Name ',File_Name)

			File_Name_robot = file_robot.get()
			#print ('Plot Robot File_Name_robot ',File_Name_robot)

			flg_mov = var_mov.get()
			#print ('Plot Robot flg_mov ',flg_mov)

			flg_sensor = var_sensor.get()
			#print ('Plot Robot flg_sensor ',flg_sensor)

			#delay = float(delay.get())
			delay = 0.0
			#print ('Plot Robot delay ',delay)

			read_file_map(0)
			read_file(1)

			RobotButton['bg'] = 'green'
			RobotButton['activebackground'] = 'green'


		# Plot Robot button
		RobotButton = tk.Button(topLevelWindow, width = 20, text = 'Plot Robot Behavior', bg = 'green', activebackground = 'green', command = togglePlotRobot)



#---------------------------------------- Values' fields -----------------------------------------------------------------------------

      		# Number of sensors 
		label_num_sensors = tk.Label(topLevelWindow,text =  'Num. Sensors')
		num_sensors = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		num_sensors.insert ( 0, '16' )
		num_sensors_values = num_sensors.get()
      		# Origen angle sensor 
		label_origen_angle = tk.Label(topLevelWindow,text =  'Origen angle sensor ')
		origen_angle = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		origen_angle.insert ( 0, '-2.3561' )
		origen_angle_value = origen_angle.get()
		# Range angle sensor 
		label_range_angle = tk.Label(topLevelWindow,text =  'Range angle sensor ')
		range_angle = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		range_angle.insert ( 0, '4.7122' )
		range_angle_value = range_angle.get()
      		# Robot's magnitude advance  
		label_advance_robot = tk.Label(topLevelWindow,text =  "Robot's magnitude advance")
		advance_robot = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		advance_robot.insert(0,'0.030')
		advance_robot_value = advance_robot.get()
		original_value = advance_robot_value
		 # Robot's maximum angle  
		label_max_angle_robot = tk.Label(topLevelWindow,text =  "Robot's maximum turn angle")
		max_angle_robot = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		max_angle_robot.insert(0,'0.7854')
		max_angle_robot_value = max_angle_robot.get()
      		# Robot's radio  
		label_radio_robot = tk.Label(topLevelWindow,text =  "Robot's radio")
		radio_robot = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		radio_robot.insert ( 0, '0.02' )
		radio_robot_value = radio_robot.get()
      		# Robot's pose x  
		label_robot_posex = tk.Label(topLevelWindow,text =  "Robot's pose x")
		robot_posex = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		robot_posex.insert ( 0, '4.000' )
		pose_x_value = robot_posex.get()
      		# Robot's pose y  
		label_robot_posey = tk.Label(topLevelWindow,text =  "Robot's pose y")
		robot_posey = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		robot_posey.insert ( 0, '5.000' )
		pose_y_value = robot_posey.get()
      		# Robot's angle  
		label_robot_angle = tk.Label(topLevelWindow,text =  "Robot's angle")
		robot_angle = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		robot_angle.insert ( 0, '0.000' )
		pose_tetha = robot_angle.get()
      		# Robot's command  
		label_robot_command = tk.Label(topLevelWindow,text =  "Robot's command")
		robot_command = tk.Entry(topLevelWindow, width = 40, foreground='white',background='black')
		robot_command.insert ( 0,"../motion_planner/GoTo_State_Machine")
		robot_command_value = robot_command.get()
      		# Number of steps  
		label_number_steps = tk.Label(topLevelWindow,text =  "Number of Steps")
		number_steps = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		number_steps.insert ( 0, str(number_steps_total) )
		number_steps_value = number_steps.get()
      		# Selection of behavior  
		label_selection = tk.Label(topLevelWindow,text =  "          Behavior Selection ---------------------->>>>")
		selection = tk.Entry(topLevelWindow, width = 8, foreground='green',background='yellow')
		selection.insert ( 0,str(num_bh) )
		selection_value = selection.get()
		BEHAVIOR = selection.get()
      		# Largest value sensor  
		label_largest = tk.Label(topLevelWindow,text =  "Largest value sensor")
		largest = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		largest.insert ( 0, '0.1' )
		largest_value = largest.get()
		# Evaluation Individual 
		label_evaluation_individual = tk.Label(topLevelWindow,text =  'Evaluation Individual')
		evaluation_ind = tk.Entry(topLevelWindow, width = 6, foreground='white',background='black')
		evaluation_ind.insert ( 0, '0' )
		# Noise percentage advance 
		label_noise_percentage_advance = tk.Label(topLevelWindow,text =  'Noise percentage advance')
		noise_percentage_advance = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_advance.insert ( 0, '0.1' )
		noise_percentage_advance_value = noise_percentage_advance.get()
		# Noise percentage angle 
		label_noise_percentage_angle = tk.Label(topLevelWindow,text =  'Noise percentage angle')
		noise_percentage_angle = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_angle.insert ( 0, '0.05' )
		#noise_percentage_angle_value = noise_percentage_angle.get()
		# Noise percentage range 
		label_noise_percentage_lidar = tk.Label(topLevelWindow,text =  'Noise percentage lidar')
		noise_percentage_lidar = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_lidar.insert ( 0, '0.01' )
		noise_percentage_lidar_value = noise_percentage_lidar.get()
		# Noise percentage range standard deviation 
		label_noise_percentage_lidar_sd = tk.Label(topLevelWindow,text =  'Noise percentage lidar sigma')
		noise_percentage_lidar_sd = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_lidar_sd.insert ( 0, '0.001' )
		noise_percentage_lidar_sd_value = noise_percentage_lidar_sd.get()
		# Noise percentage light intensity 
		label_noise_percentage_light_intensity = tk.Label(topLevelWindow,text =  'Noise percentage light intensity')
		noise_percentage_light_intensity = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_light_intensity.insert ( 0, '0.1' )
		noise_percentage_light_intensity_value = noise_percentage_light_intensity.get()
		# Noise percentage light quadrant 
		label_noise_percentage_light_quadrant = tk.Label(topLevelWindow,text =  'Noise percentage position light source')
		noise_percentage_light_quadrant = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		noise_percentage_light_quadrant.insert ( 0, '0.01' )
		#noise_percentage_light_quadrant = self.noise_percentage_light_quadrant.get()
		# Number of movable obstacles  
		label_movable = tk.Label(topLevelWindow,text =  "Number of movable obstacles")
		movable = tk.Entry(topLevelWindow, width = 8, foreground='white',background='black')
		movable.insert ( 0, '0' )
		number_unk_value = movable.get()

		# Add noise
		flg_noise = add_noise.get()
		togglePlotMap()
		##print ("noise ",str(flg_noise))

	

#--------------------------- G R I D S ------------------------------------------------------------- 


		#self.label_robot_command.grid({'row':0, 'column': 0})        
		#self.robot_command.grid({'row':0, 'column': 1})        
		label_path.grid({'row':1, 'column': 0})        
		path.grid({'row':1, 'column': 1})        
		label_file.grid({'row':2, 'column': 0})        
		file.grid({'row':2, 'column': 1})        
		label_file_robot.grid({'row':3, 'column': 0})        
		file_robot.grid({'row':3, 'column': 1})        
		MapButton.grid({'row':5, 'column': 0})
		RobotButton.grid({'row':6, 'column': 0})
		Movement.grid({'row':0, 'column': 2})
		sensor.grid({'row':1, 'column': 2})
		noise.grid({'row':2, 'column': 2})
		method.grid({'row':3, 'column': 2})
		sensor_line.grid({'row':0, 'column': 3})
		label_num_sensors.grid({'row':1, 'column': 3})        
		num_sensors.grid({'row':1, 'column': 4})        
		label_origen_angle.grid({'row':2, 'column': 3})        
		origen_angle.grid({'row':2, 'column': 4})        
		label_range_angle.grid({'row':3, 'column': 3})        
		range_angle.grid({'row':3, 'column': 4})        
		label_radio_robot.grid({'row':4, 'column': 3})        
		radio_robot.grid({'row':4, 'column': 4})        
		label_advance_robot.grid({'row':5, 'column': 3})        
		advance_robot.grid({'row':5, 'column': 4})        
		label_max_angle_robot.grid({'row':6, 'column': 3})        
		max_angle_robot.grid({'row':6, 'column': 4})        
		label_robot_posex.grid({'row':4, 'column': 1})        
		robot_posex.grid({'row':4, 'column': 2})        
		label_robot_posey.grid({'row':5, 'column': 1})        
		robot_posey.grid({'row':5, 'column': 2})        
		label_robot_angle.grid({'row':6, 'column': 1})        
		robot_angle.grid({'row':6, 'column': 2})        
		label_number_steps.grid({'row':7, 'column': 3})        
		number_steps.grid({'row':7, 'column': 4})        
		label_selection.grid({'row':7, 'column': 1})        
		selection.grid({'row':7, 'column': 2})        
		label_largest.grid({'row':6, 'column': 5})        
		largest.grid({'row':6, 'column': 6})        
		label_evaluation_individual.grid({'row':7, 'column': 5})        
		evaluation_ind.grid({'row':7, 'column': 6})        
		label_noise_percentage_advance.grid({'row':0, 'column': 5})        
		noise_percentage_advance.grid({'row':0, 'column': 6})        
		label_noise_percentage_angle.grid({'row':1, 'column': 5})        
		noise_percentage_angle.grid({'row':1, 'column': 6})        
		label_noise_percentage_lidar.grid({'row':2, 'column': 5})        
		noise_percentage_lidar.grid({'row':2, 'column': 6})        
		label_noise_percentage_lidar_sd.grid({'row':3, 'column': 5})        
		noise_percentage_lidar_sd.grid({'row':3, 'column': 6})        
		label_noise_percentage_light_intensity.grid({'row':4, 'column': 5})        
		noise_percentage_light_intensity.grid({'row':4, 'column': 6})        
		label_noise_percentage_light_quadrant.grid({'row':5, 'column': 5})        
		noise_percentage_light_quadrant.grid({'row':5, 'column': 6})        
		buttonPlotTopological.grid({'row':7, 'column': 0})
		buttonStop.grid({'row':8, 'column': 0})
		buttonHelp.grid({'row':8, 'column': 1})
		buttonClips.grid({'row':8, 'column': 2})
		buttonExample.grid({'row':8, 'column': 3})
    
		if num_behavior == 0:
			self.ButtonPath(1)




		def togglePlotExecute(flg_output,BEHAVIOR):

			global mouse_1_x
			global mouse_1_y
			global mouse_3_x
			global mouse_3_y
			global angle_robot
			global sensor
			#global num_sensors
			global flg_noise
			global start_angle
			#global range_angle
			global flg_execute
			global pose_x
			global pose_y
			#global radio_robot
			global advance_robot
			#global max_angle_robot
			#global robot_command
			global flg_plt
			global number_unk
			global flg_start_clips


			flg_execute = 1

			#ExecuteButton['bg'] = 'red'
			#ExecuteButton['activebackground'] = 'red'

			C.update_idletasks()

			flg_noise = add_noise.get()
			num_sensors_values = num_sensors.get()
			origen_angle_value = origen_angle.get()
			range_angle_value = range_angle.get()
			angle_robot_value = float(robot_angle.get())
			radio_robot_value = float(radio_robot.get())
			advance_robot_value = float(advance_robot.get())
			max_angle_robot_value = float(max_angle_robot.get())
			flg_sensor_value = var_sensor.get()
			number_steps_value = number_steps.get()
			method_top = method_search.get()
			#selection.delete(0, END)
			#selection.insert ( 0,str(num_bh) )
			selection_value = selection.get()
			largest_value = largest.get()
			PATH = path.get()
			#print ('Plot Robot PATH ',PATH)
			File_Name = file.get()
			#print ('Plot Robot File_Name ',File_Name)
			File_Name_robot = file_robot.get()
			#print ('Plot Robot File_Name_robot ',File_Name_robot)
			#print ('selection ' + str(selection))
			vq = 0
			size_vq = 0
			pr_out = 0
			number_unk = 0
			nn_rec = 0

			if int(selection_value) == 8 or int(selection_value) == 9:
				print ("Start CLIPS ",flg_start_clips)
				if flg_start_clips == 1:
					#command = "~/tcpclips60-master/start.sh"
					#command = "~/develop/tcpclips60/start.sh"
					command = "../../start_planning.sh"
					print ("Start CLIPS ",command)
					status = os.system(command)
					print("status ",status)
					flg_start_clips= 0
					time.sleep(1.00)
				advance_robot.delete(0, END)
				advance_robot.insert(0,'0.020')
			else:
				advance_robot.delete(0, END)
				advance_robot.insert(0,str(original_value))

			advance_robot_value = float(advance_robot.get())



			if flg_output == 1:
				File_Output = File_Name + '_' + BEHAVIOR
			else:
				File_Output = File_Name

			#print ("Behavior " + BEHAVIOR + " flg_out " + str(flg_output))

			robot_command_value = robot_command.get()
			origin = " -x " + str(mouse_1_x) + " -y " + str(mouse_1_y) + " -a " + str(angle_robot_value)
			destination = " -v " + str(mouse_3_x) + " -z " + str(mouse_3_y)
			rest = " -s " + sensor + " -n " + num_sensors_values + " -t " + origen_angle_value + " -r " + range_angle_value + " -radio " + str(radio_robot_value) + " -advance " + str(advance_robot_value) + " -MaxAngle " + str(max_angle_robot_value) + " -steps " + number_steps_value + " -selection " + selection_value + " -largest " + largest_value + " -p " + PATH + " -e " + File_Name + " -noise " + str(flg_noise) + " -vq " + str(vq) + " -size_vq " + str(size_vq) + " -pr_out " + str(pr_out) + " -nn_rec " + str(nn_rec) + " -search " + str(method_top)

			#print("origin ",origin)
			#print("destination ",destination)
			#print("rest ",rest)

			#COMMAND ROBOT
			command = robot_command_value + origin + destination + rest + " -out_file " + File_Output + " -nn_unk " + str(number_unk)  + " > " + PATH + "test_" + BEHAVIOR + ".dat"
			#command = robot_command_value + origin + destination + rest + " -out_file " + File_Output + " -nn_unk " + str(number_unk)

			print ("Robot Command: \n", command)
			status = os.system(command)

			if flg_plt == 1:
				C.update_idletasks()

				flg_mov = var_mov.get()
				#print ('Plot Robot flg_mov ',flg_mov)

				flg_sensor = var_sensor.get()
				#print ('Plot Robot flg_sensor ',flg_sensor)

				#delay = float(delay.get())
				delay = 0
				#print ('Plot Robot delay ',delay)

				read_file_map(0)
				read_file(1)


				#ExecuteButton['bg'] = 'green'
				#ExecuteButton['activebackground'] = 'green'

			mouse_1_x = pose_x
			mouse_1_y = pose_y




		def readResultFile(File_Results,File_Constants,num):

			global advance_robot;
			global number_steps_total
			global num_steps


			class Cnts:

				def __init__(self):

					self.K0 = 1.0
					self.K1 = 10.0 * self.K0 #0.1
					self.K2 = 2.0 * self.K0 #10.0  
					self.K3 = 1.0 #0.0
					self.K4 = 60.0 #60.0
					self.K5 = 0.0 #0.1
					self.K6 = 1.0 #1.0
					self.K7 = 0.0 #.10
					self.K8 = 11.0 * self.K0 #.10
					self.K9 = 21.0 * self.K0 #.10


			class Constants:

				def __init__( File_Constants):

					file_constants = open(File_Constants, 'r')
					#print ('***  File_Constants **** ',File_Constantsi)

					#evaluation = K1*(dif_o) + K2/(dif_d) + K3*dif_o_d/num_steps + 
					#             K4/num_backwards + K5/num_turns + 
					#             K6*/num_stops + K7*num_advance

					for line in file_constants:
						print (line,)
						words = line.split()
						#data = words
						#print (len(words))
						#for i in range(0, len(words)):
										#print words[i]

						if len(words) > 1:
							if words[0] == "#":
								xo = 0
							elif words[0] == "K1":
								K1 = float(words[1])
								print ('K1 ', str(K1))
							elif words[0] == "K2":
								K2 = float(words[1])
								#print ('K2 ',str(K2))
							elif words[0] == "K3":
								K3 = float(words[1])
								#print ('K3 ',str(K3))
							elif words[0] == "K4":
								K4 = float(words[1])
								#print ('K4 ',str(K4))
							elif words[0] == "K5":
								K5 = float(words[1])
								#print ('K5 ',str(K5))
							elif words[0] == "K6":
								K6 = float(words[1])
								#print ('K6 ',str(K6))
							elif words[0] == "K7":
								K7 = float(words[1])
								#print 'K7 ',str(K7)

					file_constants.close()


			Ct = Cnts()


			#print ('num ',str(num))
			#if num == 0:
				#Ct = Constants(File_Constants)



			file_results = open(File_Results, 'r')
			#print ('File_Results ',File_Results)

			num_backwards = 1
			num_advance = 1
			num_turns = 1
			num_stops = 1
			num_st = 1
			num_collisions = 1
			previous_angle = 0.0
			previous_advance = 0.0
			previous_x = 0.0
			previous_y = 0.0
			THRESHOLD_MOVEMENT = 0.007
			num_steps_total = float(number_steps.get())

			threshold_noise = float(advance_robot_value)/2
			threshold_angle = float(max_angle_robot_value)/2
			#print ("threshold noise ",str(threshold_noise))
			#print ("threshold angle ",str(threshold_angle))
			PATH = path.get()
			#print ('Evaluate Robot PATH ',PATH)
			#File_Name = file.get()

			xx=[]
			yy=[]


			for line in file_results:
				#print (line,)
				words = line.split()
				data = words
				#print (len(words))
				#for i in range(0, len(words)):
				#print words[i]
				if len(words) > 1:
					if words[1] == "origen":
						xo = float(words[2])
						yo = float(words[3])
						zo = float(words[4])
						#print ('xo ',xo, ' yo ',yo,' zo ',zo)
					elif words[1] == "robot":
						x= float(words[3])
						xx.append(x)
						y= float(words[4])
						yy.append(y)
						tetha= float(words[5])
						#print ('robot x ',x, ' y ',y,' angle ',tetha)
						difx = (x-previous_x)
						dify = (y-previous_y)
						mag = math.sqrt(pow(difx,2)+pow(dify,2))
						if mag < THRESHOLD_MOVEMENT:
							num_stops=num_stops + 1
						previous_x=x
						previous_y=y
						num_st = num_st+1
						#print ("Num_steps ",str(num_st)," Magnitude ", str(mag)," num_stops ",str(num_stops))
					elif words[1] == "destination":
						xd= float(words[2])
						yd= float(words[3])
						#print ('xd ',xd, ' yd',yd)
					elif words[1] == "collision":
						num_collisions = num_collisions + 1
						#print ('num.collisions ',str(num_collisions))
					elif words[1] == "distance":
						distance= float(words[2])
						#print ('distance ',distance)
					elif words[1] == "num_steps":
						num_steps= float(words[2])-1
						#print ('num_steps ',num_steps)
					elif words[1] == "movement":
						angle= float(words[2])
						advance= float(words[3])
						#print ('angle ',str(angle),' advance ',str(advance))
						if(advance < -threshold_noise):
							num_backwards=num_backwards + 1
							#print (' advance ',str(advance))

						if(abs(angle) >= threshold_angle and abs(previous_angle) >= threshold_angle):
							num_turns=num_turns + 1

						if(advance >= threshold_noise and previous_advance >= threshold_noise):
							num_advance=num_advance + 1
							#print (' advance ',str(advance),' prev. advance ',str(previous_advance))

						#if(angle == 0.0 and advance == 0.0):
							#num_stops=num_stops + 1

						previous_angle = angle
						previous_advance = advance


			sdx = np.std(xx)
			sdy = np.std(yy)
			sd = sdy + sdx
			#print ('Standard Deviation ', str(sd))
			#print ("Num_stops ",str(num_stops))
			#print ('Num.collisions ',str(num_collisions))
			#print ("Num_backwards ",str(num_backwards))

			#print 'final position x ',x, ' y ',y,' angle ',tetha
			dif_o= math.sqrt( math.pow( (xo -x),2)+math.pow( (yo -y),2))
			dif_d= math.sqrt(math.pow( (xd -x),2)+math.pow( (yd -y),2))

			command = "../Dijkstra/Dijkstra -x " + str(x) + " -y " + str(y) + " -v " + str(xd) + " -z " + str(yd) + " -p " + PATH + " -e " + File_Name + " > " + PATH + "rslt_" + BEHAVIOR + ".dat"
			#print ("Dijkstra command: ", command)
			status = os.system(command)
			
			#command = "tail rslt.dat"
			#status = os.system(command)
			FILE = PATH + "rslt_" + BEHAVIOR + ".dat"
			if(os.path.isfile(FILE)):
				file = open(FILE, 'r')
				dummy = file.readline()
				distance_Dijkstra = float(file.readline())
				#print ('dif_d ',str(dif_d), ' distance_Dijkstra ',str(distance_Dijkstra))
				file.close()
			else:
				distance_Dijkstra = 0
				print ("File does not exists " + FILE)

			dif_d = dif_d + distance_Dijkstra
			#dif_d = dif_d + 60*distance_Dijkstra
			dif_o_d= math.sqrt(math.pow( (xd -xo),2)+math.pow( (yd -yo),2))
			#print ('dif_d ',str(dif_d))
			##print ('dif_o ',str(dif_o))


			evaluation = Ct.K1*abs((number_steps_total - num_steps + 1)*dif_o) + abs(number_steps_total - num_steps + 1)/(Ct.K2*dif_d) + abs(dif_o_d/(Ct.K3*num_steps)) + abs(number_steps_total - num_steps+1)/(Ct.K4*num_backwards) + Ct.K8*sd + abs(number_steps_total - num_steps+1)/(Ct.K6*num_stops) + abs(number_steps_total - num_steps+1)/(Ct.K9*num_collisions)


			file_results.close()

			return evaluation


#___________________________________________________________________________________________________ 

		def callback_mouse_1(event):
			global mouse_1_x
			global mouse_1_y
			global angle_robot

			#print ("clicked 1 at", event.x, event.y)
			id = C.create_rectangle(event.x, event.y, event.x+1, event.y+1, fill= "black")
			x = (dim_x * event.x) / DIM_CANVAS_X
			y = (dim_y * (DIM_CANVAS_Y-event.y)) / DIM_CANVAS_Y
			print ("left button x ", x, " y ", y)
			mouse_1_x = x
			mouse_1_y = y
			#angle_robot=float(robot_angle.get())
			angle_robot=pose_tetha
			#angle_robot = 0 

 
		def callback_mouse_2(event):
			global mouse_2_x
			global mouse_2_y

			#print ("clicked 2 at", event.x, event.y)
			id = C.create_rectangle(event.x, event.y, event.x+1, event.y+1, fill= "green", outline='yellow')
			x = (dim_x * event.x) / DIM_CANVAS_X
			y = (dim_y * (DIM_CANVAS_Y-event.y)) / DIM_CANVAS_Y
			print ("middle button x ", x, " y ", y)
			mouse_2_x = x
			mouse_2_y = y

    

		def callback_mouse_3(event):
			global mouse_3_x
			global mouse_3_y
			global flg_plt
			global BEHAVIOR

			#print ("clicked 3 at", event.x, event.y)
			id = C.create_rectangle(event.x, event.y, event.x+1, event.y+1, fill= "red", outline='red')
			x = (dim_x * event.x) / DIM_CANVAS_X
			y = (dim_y * (DIM_CANVAS_Y-event.y)) / DIM_CANVAS_Y
			#print ("right button x ", x, " y ", y)
			mouse_3_x = x
			mouse_3_y = y

			if flg_execute == 1:
				if var_mov.get() == 0:
					C.update_idletasks()

				flg_plt = 1
				BEHAVIOR = selection.get()
				togglePlotExecute(0,BEHAVIOR)

				PATH = path.get()
				#print ('Evaluate Robot PATH ',PATH)
				File_Name = file.get()
				FILE = PATH + File_Name + '.raw'
				#print ('Evaluate Robot File_Name ',FILE)

				File_Constants = PATH + 'Constants.txt'
				partial_evaluation = readResultFile(FILE,File_Constants,0)
				print ('Evaluation ',partial_evaluation)
				evaluation_ind.delete(0, END)
				evaluation_ind.insert ( 0, str(partial_evaluation))




		C.bind("<Button-1>", callback_mouse_1)
		C.bind("<Button-2>", callback_mouse_2)
		C.bind("<Button-3>", callback_mouse_3)


	C.pack()
	initial()



	def plot_test(self):
		global C


		id = C.create_rectangle(0, 0, DIM_CANVAS_X, DIM_CANVAS_Y, fill= "blue")
		coord = 10, 50, 40, 80
		arc = C.create_arc(coord, start=0, extent=150, fill="red")
		points = [150, 100, 200, 120, 240, 180, 210, 200, 150, 150, 100, 200]
		polygon = C.create_polygon(points, outline='black', fill='red', width=1)
		points = [1.50, 1.00, 2.00, 1.20, 2.40, 1.80, 2.10, 2.00, 1.50, 1.50, 1.00, 2.00]
		self.plot_polygon(6, points)
		oval = C.create_oval(300, 300, 380, 380, outline="black", fill="red", width=2)
		line = C.create_line(30, 300, 100, 280,  fill="red", arrow="last")
		self.plot_robot()


	def plot_polygon_green(self,num, data):
        	global C

        	XY= data
        	#print ("plot_polygon ",num)
        	for i in range(0, num):
                	j=i*2

                	#print ("data j ",j," x",data[j]," y ",data[j+1])
                	X = ( DIM_CANVAS_X * data[j] ) / dim_x
                	Y = DIM_CANVAS_Y - ( DIM_CANVAS_Y * data[j+1] ) / dim_y
                	XY[j]=X;
                	XY[j+1]=Y;
                	#print ("j ",j," X",XY[j]," Y ",XY[j+1])

        	polygon = C.create_polygon(XY, outline='green', fill='blue', width=1)
        	return polygon





#-----------------------------------------------------
#  MAIN

if __name__ == '__main__':
	gui_planner = PLANNER()

#    if flg_start_clips == 1:
	#command = "~/tcpclips60-master/start.sh"
	#print ("Start CLIPS ",command)
	#status = os.system(command)
	#print("status ",status)
	#flg_start_clips= 0

	tk.mainloop()

