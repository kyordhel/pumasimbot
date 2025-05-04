
/***********************************************
*                                              *
*      oracle.h	                               *
*                                              *
*      Jesus Savage                            *
*      Mauricio Matamoros                      *
*                                              *
*              Bio-Robotics Laboratory         *
*              UNAM, 2025                      *
*                                              *
*                                              *
************************************************/


#include "reply.h"
#include "request.h"
#include "clipsclient.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <ctime>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>


/* ** ********************************************************
* Global variables
* *** *******************************************************/
std::shared_ptr<ClipsClient> clientPtr;
std::string answer_clp;
std::timed_mutex mtx;
std::condition_variable_any cv;
bool flag_clips = false;


/* ** ********************************************************
* Prototypes
* *** *******************************************************/
void connect(const std::string& address, uint16_t port);

static inline void reqLoadFile(const std::string& file);
static inline void reqSetPath(const std::string& path);
static inline void reqReset();
static inline void reqClear();
static inline void reqRun();
static inline void reqAssert(const std::string& fact);
static inline void reqRetract(const std::string& fact);
void cleanup();
void messageReceivedHandler(const ClipsClientPtr& client, const std::string& msg);

static inline std::string wait_answer_clips();
static inline void sleep_ms(size_t ms);

/* ** ********************************************************
* Function definitions
* *** *******************************************************/

void cleanup(){
        clientPtr->disconnect();
}


/**
 * Requests CLIPS to assert a fact.
 */
void reqAssert(const std::string& fact){
        clientPtr->assertFact( fact );
}


/**
 * Requests CLIPS to change the path
 */
void reqSetPath(const std::string& path){
        clientPtr->setPath( path );
}


/**
 * Requests CLIPS to reset the KDB
 */
void reqReset(){
        clientPtr->reset();
}

/**
 * Requests CLIPS to clear & wipe out the KDB
 */
void reqClear(){
        clientPtr->clear();
}


/**
 * Requests CLIPS to run the KDB engine
 */
void reqRun(){
        clientPtr->run(-1);
}


/**
 * Requests CLIPS to load the specified file
 * @param file Path to file to load
 */
void reqLoadFile(const std::string& file){
        reqClear();
        clientPtr->loadFile(file);
}

/**
 * Fired by ClipsClient whenever a message is received.
 * @param client The client that triggered the event
 * @param msg    The received message
 */
void messageReceivedHandler(const ClipsClientPtr& client, const std::string& msg){
        std::unique_lock<std::timed_mutex> lock(mtx); // Prevents two threads to access the mutex, protecting answer_clp
        answer_clp = msg;
        flag_clips = true;
        cv.notify_one();
}


/**
 * Connects clipsclient to a remote CLIPSServer
 * @param address The IP address hosting CLIPSServer
 * @param port    CLIPSServer's connection port
 */
void connect(const std::string& address, uint16_t port){
        if(!clientPtr->connect(address, port)){
                fprintf(stderr, "Could not connect to CLIPS on %s:%u.\n", address.c_str(), port);
                fprintf(stderr, "Run the server and pass the right parameters.\n");
                std::exit(-1);
        }
        clientPtr->addMessageReceivedHandler(&messageReceivedHandler);
        printf("Connected to CLIPS server\n");
}

/**
 * Sleeps the current execution thread for the specified amount of time
 * @param ms The amount of time in milliseconds
 */
static inline void sleep_ms(size_t ms){
        std::this_thread::sleep_for( std::chrono::milliseconds(ms) );
}



std::string wait_answer_clips(){
        std::unique_lock<std::timed_mutex> lock(mtx); // Prevents two threads to access the mutex, protecting answer_clp
        cv.wait(lock, []{ return flag_clips; }); // Releases the lock and waits until the CV is signaled/notified
        flag_clips = false;
        return answer_clp;
}


// It starts the communication with the CLIPS node
int start_clips(char *clips_file){

        if(clientPtr) return 1;

        bool init_kdb = false;
        std::string file;
        std::string result;

	#ifdef DEBUG
        std::cout << "Starting CLIPS" << std::endl;
	printf("Clips_file %s\n",clips_file);
	#endif

        clientPtr = ClipsClient::create();
        connect("127.0.0.1", 5000);
        reqSetPath("../../../../pumasimbot/behaviors/");
        reqLoadFile(clips_file);
        reqReset();
        reqRun();
        return 1;
}


//float oracle_clips(float intensity, int  dest,int obs ,movement *movements,float Max_Advance ,float Max_Twist){
AdvanceAngle oracle_clips(char *clips_file,float intensity, int  dest, int obs, float Max_Advance, float Max_Twist, int *flg_result){

 static int j=1;
 char str1[300];
 char str[1000];
 char answer_clips[1000];
 static int init_flg=1;
 std::string result;
 char accion[20];
 float rotation,advance;
 int num;
 float status = 0.0;
 AdvanceAngle gen_vector;
 int dummy;


 if(init_flg==1){

       // It starts the communication with the Clips node
       start_clips(clips_file);

       reqAssert("(alive clips)");
       printf("publish alive clips \n");
       reqRun();
       // it waits from an answer fron Clips
       strcpy( answer_clips, wait_answer_clips().c_str() );
       printf("Answer CLIPS %s\n",answer_clips);


       // It sends the maximum rotation and advance of the robot
       strcpy(str1,"(max-advance ");
       sprintf(str,"%s %f max-rotation %f",str1,Max_Advance,Max_Twist);
       strcat(str,")");
       printf("Send fact %s\n",str);
       reqAssert(str);
       reqRun();
       // it waits from an answer fron Clips
       strcpy(answer_clips, wait_answer_clips().c_str() );
       printf("Answer CLIPS %s\n",answer_clips);

       rotation = 0.0;
       advance = 0.0;
       printf("rotation %f advance %f\n",rotation,advance);
       init_flg=0;


  }
  else{

       // It loads to the Clips node a fact of the quantized sensory data
       strcpy(str1,"(step ");
       sprintf(str,"%s %d intensity %f obs %d dest %d",str1,j,intensity,obs,dest);
       strcat(str,")");
       printf("Send fact %s\n",str);
       //sleep_ms(100);
       reqAssert(str);
       
       reqRun();
       // it waits from an answer fron Clips
       strcpy(answer_clips, wait_answer_clips().c_str() );
       printf("Answer CLIPS %s\n",answer_clips);
       j++;

       sscanf(answer_clips,"%s%d%f%f%f",accion,&num,&rotation,&advance,&status);
       printf("rotation %f advance %f status %f\n",rotation,advance,status);
      
       if (status == 1.0){
                        printf("\n **************** Reached light source ******************************\n");
			*flg_result=1;
       }
       else *flg_result=0;


      //printf("Type a number -> ");
      //scanf("%d",&dummy);

  }
	
  gen_vector.distance=advance;
  gen_vector.angle=rotation;

  return gen_vector;

}

