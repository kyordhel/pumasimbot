/***********************************************

*                                              *
*      action_planner.h                        *
*                                              *
*      Jesus Savage                            *
*                                              *
*              Bio-Robotics Laboratory         *
*              UNAM, 2025                      *
*                                              *
*                                              *
************************************************/

#include <iostream>
#include <vector>
#include <ctime>
#include <map>
#include <sstream>
#include <string>

#ifndef PI
#define PI 3.1415926535f
#endif

int flg_msg = 0;



// It starts the communication with the CLIPS socket
int start_clips_node_action_planner(){
       
 char answer_clips[1000];
 char clips_file[300];
 bool init_kdb = false;
 std::string file;
 std::string result;


 //It Checks if the clips client already exists
 if(clientPtr) return 1;
 
 //It creates the clips client
 clientPtr = ClipsClient::create();
 
 //It sets the IP and port connection
 connect("127.0.0.1", 5000);

 //it sets the clips path 
 reqSetPath("ViRBot_Planning_Sockets/");
 //reqSetPath("/home/savage/pumasimbot/action_planner/ViRBot_Planning_Sockets");
 //reqSetPath("~/pumasimbot/action_planner/ViRBot_Planning_Sockets");
 //reqSetPath("");

 //it loads the clips batch file
 strcpy(clips_file,"example_planning.dat");
 #ifdef DEBUG
 std::cout << "Starting CLIPS" << std::endl;
 printf("Clips_file %s\n",clips_file);
 #endif
 reqLoadFile(clips_file);

 //it resets the clips client 
 reqReset();
 
 //It sends a fact to check if clips is alive
 reqAssert("(alive clips)");
 printf("\nsending alive clips\n");

 //It runs clips
 reqRun();

 //it waits from an answer fron Clips
 strcpy( answer_clips, wait_answer_clips().c_str() );
 printf("Answer CLIPS %s\n",answer_clips);

 return 1;

}




int action_planner(float px, float py, float theta, Actions *plan){

       static int mini_sm=1;
       static char object_name[200];
       static int init_flg=1;
       std::string result;
       static int i=0,j=0,k=0;
       char str[300];
       char action[30];
       char ROS_System[30];
       char room[30];
       char zone[30];
       static float x,y,z=0.0;
       static char arm[30];
       static char object[100];
       static float distance=1.0;
       static float angle=0.0;
       static int num_plans;
       char answer_clips[1000];

       //static char plan[100][300];



       if(init_flg==1){

       		// It starts the communication with the Clips node
       		start_clips_node_action_planner();
       		reqAssert("(start action-planning)");
		printf("start action-planning\n");
       		reqRun();
       
		// it waits from an answer fron Clips
       		strcpy( answer_clips, wait_answer_clips().c_str() );
       		printf("\nAnswer CLIPS %s\n",answer_clips);

		init_flg=0;
       }

	
       i++;
       sprintf(str,"(step %d )",i);
       printf("\nSend fact %s\n",str);
       reqAssert(str);
       reqRun();
	// it waits from an answer fron Clips
       strcpy( answer_clips, wait_answer_clips().c_str() );
       printf("\nAnswer CLIPS %s\n",answer_clips);


       sscanf(answer_clips,"%s %s",ROS_System,action);
       printf("ROS_System %s action %s\n",ROS_System,action);

       if(strcmp(action,"num_plans")==0){
       		sscanf(answer_clips,"%s %s %d",ROS_System,action,&num_plans);
       		printf("num_plans %d\n",num_plans);
		for(k=0; k< num_plans;k++){
       			sprintf(str,"(step %d )",num_plans-k);
       			//printf("\nSend fact %s\n",str);
       			reqAssert(str);
       			reqRun();
			// it waits from an answer fron Clips
       			strcpy( answer_clips, wait_answer_clips().c_str() );
       			//printf("\nAnswer CLIPS %s\n",answer_clips);
       			//printf("PLAN received: %d %s",num_plans-k,answer_clips);
			strcpy(plan->action_plan[num_plans-k],answer_clips);
		}

		for(k=1; k<= num_plans;k++){
			printf("%d %s",k,plan->action_plan[k]);
		}
       }


 return num_plans;

}



void get_coordinates_rooms(Rooms rooms, int num, char *room, char * zone, float *x, float *y){

 int j;
 float tmpx, tmpy;

 for(j=1;j<num;j++){

        printf("room %d %s\n",j,rooms.name[j]);
        if(strcmp(rooms.name[j],room) == 0){
                *x=rooms.x[j];
                *y=rooms.y[j];
                break;
        }

 }

}


void get_coordinates_objects(Objects objects, int num, char *object, float *x, float *y){

 int j;
 float tmpx, tmpy;

 for(j=1;j<num;j++){

        printf("object %d %s\n",j,objects.name[j]);
        if(strcmp(objects.name[j],object) == 0){
                *x=objects.x[j];
                *y=objects.y[j];
                break;
        }

 }

}


void modify_coordinates_objects(Objects *objects, int num, char *object, float x, float y){

 int j;
 float tmpx, tmpy;

 for(j=1;j<num;j++){

        printf("object %d %s\n",j,objects->name[j]);
        if(strcmp(objects->name[j],object) == 0){
                objects->x[j]=x;
                objects->y[j]=y;
                break;
        }

 }

}



void read_initial_state(FILE *fpw,Rooms *rooms, Objects *objects, Furniture *furniture, int *r, int *o, int *f) {

 FILE *fpr;
 char file_obj[350];
 char tmp[300];
 char name[300];
 char room[300];
 float x,y,z;
 int flg;
 int jr=1;
 int jo=1;
 int jf=1;


 strcpy(file_obj,"../action_planner/ViRBot_Planning_Sockets/virbot_initial_state.txt");
 printf("\nObjects file: %s\n",file_obj);
 if((fpr=fopen(file_obj,"r")) == NULL){
        printf("File %s can not be open\n",file_obj);
	exit(1);
 }
 else while(fscanf(fpr,"%s",tmp)==1){
        if(strcmp(tmp,"room")==0){
                flg=fscanf(fpr,"%s%f%f%f",name,&x,&y,&z);
                strcpy(rooms->name[jr],name);
                rooms->x[jr]=x;
                rooms->y[jr]=y;
                rooms->z[jr]=z;
                printf("Room %s x %f y %f z %f\n",rooms->name[jr],rooms->x[jr],rooms->y[jr],rooms->z[jr]);
                jr++;
        }
        else if(strcmp(tmp,"object")==0){
                flg=fscanf(fpr,"%s%s%f%f%f",name,room,&x,&y,&z);
                strcpy(objects->name[jo],name);
                strcpy(objects->room[jo],room);
                objects->x[jo]=x;
                objects->y[jo]=y;
                objects->z[jo]=z;
                printf("Object %s room %s x %f y %f z %f\n",objects->name[jo],objects->room[jo],objects->x[jo],objects->y[jo],objects->z[jo]);
		fprintf(fpw,"( object %s %f %f )\n",objects->name[jo],objects->x[jo],objects->y[jo]);
                jo++;
        }
        else if(strcmp(tmp,"furniture")==0){
                flg=fscanf(fpr,"%s%s%f%f%f",name,room,&x,&y,&z);
                strcpy(furniture->name[jf],name);
                strcpy(furniture->room[jf],room);
                furniture->x[jf]=x;
                furniture->y[jf]=y;
                furniture->z[jf]=z;
                printf("Furniture %s room %s x %f y %f z %f\n",furniture->name[jf],furniture->room[jf],furniture->x[jf],furniture->y[jf],furniture->z[jf]);
                jf++;
        }
        else printf("Item not listed %s\n",tmp);


 }

 strcpy(rooms->name[jr],"NIL");
 strcpy(objects->name[jo],"NIL");
 strcpy(furniture->name[jf],"NIL");

 *r=jr;
 *o=jo;
 *f=jf;

 fclose(fpr);

}




int exe_action(FILE *fpw, char *action, char *complete, float px, float py, float theta, coord *coord_room){

 int flg = 0;
 static int flg_initial = 1;
 float angle = 3.1416/4;
 float distance = 0.1;
 char exe_action[300];
 char room[300];
 char zone[300];
 char object[300];
 static char previous_object[300];
 static char previous_room[300];
 float x,y,z=0.0;
 char ROS_System[300];
 static Rooms rooms;
 static Objects objects;
 static Objects new_objects;
 static Furniture furniture;
 static int r,o,f;
 char plan[300];
 int j;
 static int k=1;
 int tmp;
 char str[1000];


 if(flg_initial == 1){
        read_initial_state(fpw,&rooms,&objects,&furniture,&r,&o,&f);

        flg_initial=0;
 }
 printf("Complete message %s\n",complete);

 if(strcmp(action,"goto")==0){

                // ACT-PLN goto bedroom deposit

                sscanf(complete,"%s %s %d %s %s %s",ROS_System,plan,&j,exe_action,room,zone);
		strcpy(previous_room,room);
                printf("Room %s zone %s\n",room,zone);
                get_coordinates_rooms(rooms,r,room,zone,&x,&y);
                printf("Coordinates room %s x %f y %f\n",room,x,y);

                //get_distance_theta(x,y,theta,px,py,&distance,&angle);
                //printf("COMMAND ACTION-PLANNER goto angle %f distance %f\n",angle,distance);

                // It returns the coordinates of the destination
                coord_room->xc = x;
                coord_room->yc = y;

 		sprintf(str,"(executed plan %d %s 1)",j,exe_action);
       		reqAssert(str);
       		reqRun();
       		printf("\nSend fact %s\n",str);

                flg = 1;
 }
 else if(strcmp(action,"grab")==0){

                // ACT-PLN grab object

                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("object %s\n",object);
                get_coordinates_objects(objects,o,object,&x,&y);
                printf("Coordinates object %s x %f y %f\n",object,x,y);
                fprintf(fpw,"( erase %s %f %f )\n",object,x,y);

		sprintf(str,"(executed plan %d %s 1)",j,exe_action);
       		printf("\nSend fact %s\n",str);
       		reqAssert(str);
       		reqRun();

		//printf("type a number \n");
        	//scanf("%d",&tmp);


                //get_distance_theta(x,y,theta,px,py,&distance,&angle);
                //printf("COMMAND ACTION-PLANNER goto angle %f distance %f\n",angle,distance);

                // It returns the coordinates of the destination
                //coord_room->xc = x;
                //coord_room->yc = y;

                flg = 1;
 }
 else if(strcmp(action,"drop")==0){

                // ACT-PLN drop object

                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("object %s\n",object);
                //get_coordinates_objects(objects,o,object,&x,&y);
                printf("New coordinates object %s x %f y %f\n",object,px,py);
                fprintf(fpw,"( new_object %s %f %f )\n",object,px,py);
		strcpy(new_objects.name[k],object);
		new_objects.x[k]=px;
		new_objects.y[k]=py;
		k++;
 		
		sprintf(str,"(executed plan %d %s 1)",j,exe_action);
       		printf("\nSend fact %s\n",str);
       		reqAssert(str);

		sprintf(str,"(update object %s room %s)",object,previous_room);
       		reqAssert(str);
       		reqRun();
       		printf("\nSend fact %s\n",str);

                //printf("type a number \n");
                //scanf("%d",&tmp);


                //get_distance_theta(x,y,theta,px,py,&distance,&angle);
                //printf("COMMAND ACTION-PLANNER goto angle %f distance %f\n",angle,distance);

                // It returns the coordinates of the destination
                //coord_room->xc = x;
                //coord_room->yc = y;

                flg = 1;
 }
 else if(strcmp(action,"drop_same")==0){

                // ACT-PLN drop_same place object

                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("object %s\n",object);
                //get_coordinates_objects(objects,o,object,&x,&y);
                printf("New coordinates object %s x %f y %f\n",object,px,py);
                //fprintf(fpw,"( new_object %s %f %f )\n",object,px,py);
                strcpy(new_objects.name[k],object);
                new_objects.x[k]=px;
                new_objects.y[k]=py;
                k++;

                sprintf(str,"(executed plan %d %s 1)",j,exe_action);
                printf("\nSend fact %s\n",str);
                reqAssert(str);

                //sprintf(str,"(update object %s room %s)",object,previous_room);
                //reqAssert(str);
                reqRun();
                //printf("\nSend fact %s\n",str);

                //printf("type a number \n");
                //scanf("%d",&tmp);


                //get_distance_theta(x,y,theta,px,py,&distance,&angle);
                //printf("COMMAND ACTION-PLANNER goto angle %f distance %f\n",angle,distance);

                // It returns the coordinates of the destination
                //coord_room->xc = x;
                //coord_room->yc = y;

                flg = 1;
 }
 else if(strcmp(action,"find-object")==0){
		//ACT-PLN plan 9 find-object soap

                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("find-object %s\n",object);
                sprintf(str,"(executed plan %d %s 1)",j,exe_action);
                printf("\nSend fact %s\n",str);
                reqAssert(str);

                //printf("type a number \n");
                //scanf("%d",&tmp);

                flg = 1;
 }
 else if(strcmp(action,"mv")==0){
		//ACT-PLN plan 3 mv milk
                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("mv %s\n",object);
                sprintf(str,"(executed plan %d %s 1)",j,exe_action);
                printf("\nSend fact %s\n",str);
                reqAssert(str);

                //printf("type a number \n");
                //scanf("%d",&tmp);

                flg = 1;
 }
 else if(strcmp(action,"go")==0){
                //ACT-PLN plan 3 go freescape
                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("go %s\n",object);
                sprintf(str,"(executed plan %d %s 1)",j,exe_action);
                printf("\nSend fact %s\n",str);
                reqAssert(str);

                //printf("type a number \n");
                //scanf("%d",&tmp);

                flg = 1;
 }
 else if(strcmp(action,"move")==0){
                //ACT-PLN plan 3 mv milk
                sscanf(complete,"%s %s %d %s %s",ROS_System,plan,&j,exe_action,object);
                printf("mv %s\n",object);
                sprintf(str,"(executed plan %d %s 1)",j,exe_action);
                printf("\nSend fact %s\n",str);
                reqAssert(str);

                //printf("type a number \n");
                //scanf("%d",&tmp);

                flg = 1;
 }
 else if(strcmp(action,"end")==0){
        printf("clean\n");
	fprintf(fpw,"( clean )\n");
	for(j=1;j<k;j++){
        	printf("new object %d %s %f %f\n",j,new_objects.name[j],new_objects.x[j],new_objects.y[j]);
		fprintf(fpw,"( new_object %s %f %f )\n",new_objects.name[j],new_objects.x[j],new_objects.y[j]);
		//printf("type a number \n");
        	//scanf("%d",&tmp);
        }
 }
 else{
        //printf("This action %s do not exist\n",action);
 }


 return flg;

}


