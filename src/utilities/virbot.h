#pragma once

#include "minibot/minibot.h"
#include "simulator/simulation.h"
#include "utilities/structures.h"

/* ** *****************************************************************
* Types, defs and enums
** ** ****************************************************************/
typedef int (*mvrobotFPtr_t)(FILE*, AdvanceAngle, coord*);
typedef void (*getsenvalFPtr_t)(coord, float, float, Raw*, int, float);
typedef void (*getintangFPtr_t)(coord, coord, float*, float*);

/* ** *****************************************************************
* Global function declaration
** ** ****************************************************************/
void virbot_use_real_robot();
void virbot_use_virtual_robot();

int mvrobot(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot);
int mvrobot_real(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot);
int mvrobot_virtual(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot);

void get_sensor_values(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value);
void get_sensor_values_real(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value);

void get_intensity_angle(coord coord_robot, coord coord_destination,
	float *intensity, float *light_angle);
void get_intensity_angle_real(coord coord_robot, coord coord_destination,
	float *intensity, float *light_angle);

/* ** *****************************************************************
*
* To be moved to virbot.cpp
*
** ** ****************************************************************/
static mvrobotFPtr_t mvrobot_ptr     = &mvrobot_real;
static getsenvalFPtr_t getsenval_ptr = &get_sensor_values_real;
static getintangFPtr_t getintang_ptr = &get_intensity_angle_real;

// static mvrobotFPtr_t mvrobot_ptr     = &mvrobot_virtual;
// static getsenvalFPtr_t getsenval_ptr = &get_sensor_values_virtual;
// static getintangFPtr_t getintang_ptr = &get_intensity_angle_virtual;

/* ** *****************************************************************
* Prototypes
** ** ****************************************************************/
int fix_mvrobot_collision(
	FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot,
	float dcx, float dcy, float dca
);

/* ** *****************************************************************
* Function declaration
** ** ****************************************************************/
void virbot_use_real_robot(){
	mvrobot_ptr   = &mvrobot_real;
	getsenval_ptr = &get_sensor_values_real;
	getintang_ptr = &get_intensity_angle_real;
}



void virbot_use_virtual_robot(){
	mvrobot_ptr   = &mvrobot_virtual;
	getsenval_ptr = &get_sensor_values_virtual;
	getintang_ptr = &get_intensity_angle_virtual;
}


void get_sensor_values(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value){
	if( !getsenval_ptr ) return;
	getsenval_ptr(coord_robot, start_angle, range, sensor_vector, num, largest_value);
}


void get_sensor_values_real(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value){
}



void get_intensity_angle(coord coord_robot, coord coord_destination,
	float *intensity, float *light_angle){
	if( !getintang_ptr ) return;
	getintang_ptr(coord_robot, coord_destination, intensity, light_angle);
}


void get_intensity_angle_real(coord coord_robot, coord coord_destination,
	float *intensity, float *light_angle){
}


int mvrobot(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	if(mvrobot_ptr)
		return mvrobot_ptr(fpw, DistTheta, coord_robot);
	return 0;
}


// it will move the robot the desire angle and distance
int  mvrobot_virtual(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	float dcx, dcy, dca;

	dca = DistTheta.angle;

	dcx = DistTheta.distance * cos(dca + coord_robot->anglec);
	dcy = DistTheta.distance * sin(dca + coord_robot->anglec);

	return fix_mvrobot_collision(fpw, DistTheta, coord_robot, dcx, dcy, dca);
}



int mvrobot_real(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	float dcx, dcy, dca;

	if(!mvrobot_rpc(DistTheta.distance, DistTheta.angle,  &dcx, &dcy, &dca))
		return -1;
	return fix_mvrobot_collision(fpw, DistTheta, coord_robot, dcx, dcy, dca);
}



int fix_mvrobot_collision(
	FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot,
	float dcx, float dcy, float dca
){
	int flg=0;
	int flg_unk=0;
	float new_xmv,new_ymv,new_thetamv;
	float xc,yc;
	float speed;
	coord new_coord;
	float xmv,ymv,thetamv;
	float distance, angle1;
	int dummy;
	float cnt=3.0;
	int indx=0;
	int flag = 0;
	int flag_unk = 1;


	xmv     = coord_robot->xc;
	ymv     = coord_robot->yc;
	thetamv = coord_robot->anglec;

	angle1   = DistTheta.angle;
	distance = DistTheta.distance;

	//#ifdef DEBUG
	//printf("before x:%f, y:%f,  rad:%f\n",coord_robot->xc,coord_robot->yc,coord_robot->anglec);
	//printf("Distance %f before angle1 %f\n",distance,angle1);
	//#endif

	new_thetamv = angle1 + dca;
	new_xmv     = xmv + dcx;
	new_ymv     = ymv + dcy;

	// #ifdef DEBUG
	// printf("new_thetamv %f\n",new_thetamv);
	// printf("new_xmv %f new_ymv %f\n",new_xmv,new_ymv);
	//#endif

	// it checks if the robot new position is inside an obstacle
	//flg = shs_distance_obstacle(xmv,ymv,new_xmv,new_ymv,.5,&indx);
	flg = shs_distance_obstacle(xmv, ymv, new_xmv, new_ymv, cnt*distance, &indx, 0);
	if(num_polygons_unk > 0)
		flg_unk = shs_distance_obstacle(xmv, ymv, new_xmv, new_ymv, cnt*distance, &indx, 1);

	//#ifdef DEBUG
	//printf("check inside flg %d x:%.3f, y:%.3f,  rad:%.3f polygon  %d polygon name %s\n",flg,new_xmv,new_ymv,new_thetamv,indx,polygons_wrl[indx].name);
	//#endif

	//flg = check_inside_polygon(new_xmv,new_ymv,polygons_wrl,indx);

	flag = inside_polygon(num_polygons_wrl, polygons_wrl, new_xmv, new_ymv, &indx);
	//printf("num_polygons_unk %d\n",num_polygons_unk);
	if(num_polygons_unk > 0)
		flag_unk = inside_polygon(num_polygons_unk+1, polygons_unk, new_xmv, new_ymv, &indx);

	if(angle1 > 5.75f) angle1=- (angle1 - 5.75f) ;
	if(new_thetamv > 6.2832)    new_thetamv-= 6.2832f;
	else if(new_thetamv < -0.0) new_thetamv+= 6.2832f;

	new_xmv = xmv + (float) distance*(float)cos((float) (new_thetamv));
	new_ymv = ymv + (float) distance*(float)sin((float) (new_thetamv));

	//printf("flag %d flg %d flag_unk %d flg_unk %d\n",flag,flg,flag_unk,flg_unk);
	if((flag==1 && flg == 0) && ( flag_unk==1 && flg_unk == 0)){
		coord_robot->xc=new_xmv;
		coord_robot->yc=new_ymv;
		coord_robot->anglec=new_thetamv;
	}
	else{
		#ifdef DEBUG
		printf("robot inside polygon %d x:%.3f, y:%.3f,  rad:%.3f polygon  %d polygon name %s\n",flag,new_xmv,new_ymv,new_thetamv,indx,polygons_wrl[indx].name);
		printf("The robot remains with the previous position\n");
		#endif
		//printf("The robot remains with the previous position\n");
		dummy=0;
		fprintf(fpw,"( collision obstacle )\n");
	}

	//#ifdef DEBUG
	//printf("after x:%f, y:%f,  rad:%f\n",coord_robot->xc,coord_robot->yc,coord_robot->anglec);
	//#endif

	return flg;
}

