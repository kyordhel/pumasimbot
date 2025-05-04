/********************************************************
 *                                                      *
 *                                                      *
 *      reactive_behavior.h          			*
 *                                                      *
 *              Jesus Savage                            *
 *		FI-UNAM					*
 *		2-15-2024                               *
 *                                                      *
 ********************************************************/


// Student State Machine 
AdvanceAngle reactive_behavior(Raw observations, int dest, int intensity, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 static int step=0;

 step++;
#ifdef DEBUG
 printf("\n\n **************** Reactive Behavior %d *********************\n",step);
#endif

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
#ifdef DEBUG
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
#endif
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
#ifdef DEBUG
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
#endif
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
#ifdef DEBUG
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);
#endif
 if( left_side < THRS_SENSOR) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS_SENSOR) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 obs = value;
#ifdef DEBUG
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);
#endif

 if (intensity == 1){
	// Constants STOP, TURN RIGHT, ETC, are defined in ../utilities/constants.h
	// generate_output function in ../utilities/utilities.h
	gen_vector=generate_output(STOP,Mag_Advance,max_angle);

#ifdef DEBUG
        printf("STOP\n");
	printf("\n **************** Reached light source ******************************\n");
#endif
 }
 else if (obs == 0){
	// There is not obstacle
        //gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
	//printf("FORWARD\n");

	if (dest == 0){
                // go right twice
		gen_vector=generate_output(RIGHTADVANCETWICE,Mag_Advance,max_angle);
#ifdef DEBUG
                printf("TURN RIGHT TWICE\n");
#endif
        }
        else if (dest == 1){
                // go left twice
                gen_vector=generate_output(LEFTADVANCETWICE,Mag_Advance,max_angle);
#ifdef DEBUG
                printf("TURN LEFT TWICE\n");
#endif
        }
        else if (dest == 2){
                 // go right
                 gen_vector=generate_output(RIGHTADVANCE,Mag_Advance,max_angle);
#ifdef DEBUG
                 printf("TURN RIGHT\n");
#endif
        }
        else if (dest == 3){
                // go left
                gen_vector=generate_output(LEFTADVANCE,Mag_Advance,max_angle);
#ifdef DEBUG
                printf("TURN LEFT\n");
#endif
        }
 }
 else if (obs == 1){
        // Obtacle in the right
        gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
#ifdef DEBUG
	printf("TURN LEFT\n");
#endif
 }
 else if (obs == 2){
        // obtacle in the left
	gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
#ifdef DEBUG
	printf("TURN RIGHT\n");
#endif
 }
 else if (obs == 3){
	// obstacle in the front
        gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
#ifdef DEBUG
	printf("TURN LEFT\n");
#endif
 }


 return gen_vector;

}

