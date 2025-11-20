#pragma once
#include <math.h>


#include "minibot/minibot.h"
#include "simulator/simulation.h"
#include "utilities/structures.h"

/* ** *****************************************************************
* Types, defs and enums
** ** ****************************************************************/
typedef int  (*mvrobotFPtr_t)(FILE*, AdvanceAngle, coord*);
typedef void (*getbattchFPtr_t)(float*, int*);
typedef void (*getsenvalFPtr_t)(coord, float, float, Raw*, int, float);
typedef void (*getintangFPtr_t)(coord, coord, float*, float*);

/* ** *****************************************************************
* Global function declaration
** ** ****************************************************************/
void virbot_use_real_robot();
void virbot_use_virtual_robot();
bool virbot_using_real_robot();

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

void get_battery_charge(float *voltage, int *percentage);
void get_battery_charge_real(float *voltage, int *percentage);
void get_battery_charge_virtual(float *voltage, int *percentage);

/* ** *****************************************************************
*
* To be moved to virbot.cpp
*
** ** ****************************************************************/
static mvrobotFPtr_t mvrobot_ptr     = &mvrobot_real;
static getsenvalFPtr_t getsenval_ptr = &get_sensor_values_real;
static getintangFPtr_t getintang_ptr = &get_intensity_angle_real;
static getbattchFPtr_t getbattch_ptr = &get_battery_charge_real;

// static mvrobotFPtr_t mvrobot_ptr     = &mvrobot_virtual;
// static getsenvalFPtr_t getsenval_ptr = &get_sensor_values_virtual;
// static getintangFPtr_t getintang_ptr = &get_intensity_angle_virtual;
// static getbattchFPtr_t getbattch_ptr = &get_battery_charge_virtual;

/* ** *****************************************************************
* Local Prototypes
** ** ****************************************************************/
int fix_mvrobot_collision(
	FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot,
	float dcx, float dcy, float dca
);

/* ** *****************************************************************
* Function declaration: VIRTUAL/REAL ROBOT SELECTION
** ** ****************************************************************/
bool virbot_using_real_robot(){
	return (mvrobot_ptr   == &mvrobot_real)
	    || (getsenval_ptr == &get_sensor_values_real)
	    || (getintang_ptr == &get_intensity_angle_real);
}

void virbot_use_real_robot(){
	mvrobot_ptr   = &mvrobot_real;
	getsenval_ptr = &get_sensor_values_real;
	getintang_ptr = &get_intensity_angle_real;
	getbattch_ptr = &get_battery_charge_real;
}



void virbot_use_virtual_robot(){
	mvrobot_ptr   = &mvrobot_virtual;
	getsenval_ptr = &get_sensor_values_virtual;
	getintang_ptr = &get_intensity_angle_virtual;
	getbattch_ptr = &get_battery_charge_virtual;
}


/* ** *****************************************************************
* Function declaration: WRAPPERS
** ** ****************************************************************/
void get_sensor_values(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value){
	if( !getsenval_ptr ) return;
	getsenval_ptr(coord_robot, start_angle, range, sensor_vector, num, largest_value);
}


void get_intensity_angle(coord coord_robot, coord coord_destination,
	float *intensity, float *light_angle){
	if( !getintang_ptr ) return;
	getintang_ptr(coord_robot, coord_destination, intensity, light_angle);
}


int mvrobot(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	if(mvrobot_ptr)
		return mvrobot_ptr(fpw, DistTheta, coord_robot);
	return 0;
}


void get_battery_charge(float *voltage, int *percentage){
	if( !getbattch_ptr ) return;
	getbattch_ptr(voltage, percentage);
}


/* ** *****************************************************************
* Function declaration: VIRTUAL ROBOT
** ** ****************************************************************/
// it will move the robot the desire angle and distance
int  mvrobot_virtual(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	float dcx, dcy, dca;

	dca = DistTheta.angle;

	dcx = DistTheta.distance * cos(dca + coord_robot->anglec);
	dcy = DistTheta.distance * sin(dca + coord_robot->anglec);

	return fix_mvrobot_collision(fpw, DistTheta, coord_robot, dcx, dcy, dca);
}

void get_battery_charge_virtual(float *voltage, int *percentage){
	*voltage = 7.2;
	*percentage = 100;
}


/* ** *****************************************************************
* Function declaration: REAL ROBOT
** ** ****************************************************************/
void get_sensor_values_real(coord coord_robot, float start_angle, float range,
	Raw *sensor_vector, int num, float largest_value){
	printf("get_sensor_values_real\n");
	float distance;
	float pos[5];
	float angle;
	float inc_angle;
	float x, y, theta;
	sdistr_t readings;

	pos[0] = sensor_vector[0].x = coord_robot.xc;
	pos[1] = sensor_vector[0].y = coord_robot.yc;

	read_dist_rpc(&readings);
	for(uint8_t k=0; k < readings.count; ++k) {
		if((readings.data[k].value > largest_value) || (readings.data[k].value < 0))
			readings.data[k].value = largest_value;

		sensor_vector[0].sensors[k] = readings.data[k].value;
		// Maybe add start_angle to each sensor?
		sensor_vector[0].theta[k]   = readings.data[k].angle; // + start_angle;
	}
	#ifdef DEBUG
		int pcount = 0;
		printf("Obstacle sensors:");
		for(uint8_t k=0; k < readings.count; ++k) {
			// if((readings.data[k].angle < 0) || (readings.data[k].value < 0))
				// continue;
			++pcount;
			printf("\n    (%3d°,% 0.3f)",
				(int)(180 * readings.data[k].angle / 3.141592),
				readings.data[k].value);
		}
		printf("%s", !pcount ? "None\n" : "\n");
	#endif
}


void get_intensity_angle_real(coord coord_robot, coord coord_destination, float* intensity, float* light_angle){
	float x = 0, y = 0, angle;
	minibot::slightr_t readings;

	read_light_rpc(&readings);
	uint8_t max_val_k = 0;

	for(uint8_t k=1; k < 8; ++k) {
		if((readings.data[k].angle < 0) || (readings.data[k].value < 0))
			continue; // Skips error/defective/missread
		if(readings.data[k].value > readings.data[max_val_k].value)
			max_val_k = k;
	}
	x = readings.data[max_val_k].value * cos(readings.data[max_val_k].angle);
	y = readings.data[max_val_k].value * sin(readings.data[max_val_k].angle);
	*intensity   = readings.data[max_val_k].value;
	// *light_angle = atan2(y, x) - coord_robot.anglec;
	*light_angle = atan2(y, x);
	if(angle >= 2*PI) angle-= 2*PI;
	if(angle < 0) angle+= 2*PI;
	*light_angle = angle;

	// attraction_force = dif_vectors(coord_robot,coord_destination);
	// mag = magnitude(attraction_force);
	// *intensity = 1/(mag*mag);

	#ifdef DEBUG
		int pcount = 0;
		printf("Light sensors:");
		for(uint8_t k=0; k < 8; ++k) {
			if((readings.data[k].angle < 0) || (readings.data[k].value < 0))
				continue;
			++pcount;
			printf("\n    (%3d°,% 0.1f)",
				(int)(180 * readings.data[k].angle / 3.141592),
				100 * readings.data[k].value);
		}
		printf("%s", !pcount ? "None\n" : "\n");
		printf("light source angle %f\n", *light_angle);
	#endif
}



int mvrobot_real(FILE *fpw, AdvanceAngle DistTheta, coord *coord_robot){
	float dcx, dcy, dca;
	if(!mvrobot_rpc(DistTheta.distance, DistTheta.angle,  &dcx, &dcy, &dca)){
		return -1;
	}
	return fix_mvrobot_collision(fpw, DistTheta, coord_robot, dcx, dcy, dca);
}



void get_battery_charge_real(float *voltage, int *percentage){}


/* ** *****************************************************************
* Function declaration: Helpers
** ** ****************************************************************/
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

