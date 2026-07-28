#include <mutex>
#include <chrono>
#include <thread>

#include "minibot.h"
#include "minibot/client.h"

using MinibotClient    = minibot::client::MinibotClient;
using MinibotClientPtr = minibot::client::MinibotClientPtr;

using namespace minibot;
using namespace std::literals::chrono_literals;

/* ** *****************************************************************
* File-scope global variables
* *** ****************************************************************/
// static std::recursive_mutex minibot_client_mtx;
static std::shared_ptr<MinibotClient> minibot_client = nullptr;
static std::string minibot_cli_addr;
static uint16_t    minibot_cli_port;
static bool minibot_cli_autoreconnect;
static std::shared_ptr<std::thread> autoreconnect_thread = nullptr;


/* ** *****************************************************************
* Local Prototypes
* *** ****************************************************************/
void minibot_connected_handler(const MinibotClientPtr& client);
void minibot_disconnected_handler(const MinibotClientPtr& client);
void autoreconnect_task();


/* ** *****************************************************************
* Function definitions
* *** ****************************************************************/
bool mvrobot_rpc(float dist, float angle, float *dxc, float *dyc, float *dac){
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->mv(dist, angle, *dxc, *dyc, *dac);
}


bool robot_set_pwm_rpc(float left, float right, float front, float back){
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->setPwm(left, right, front, back);
}


bool robot_set_speed_rpc(float left, float right, float front, float back){
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->setSpeed(left, right, front, back);
}


bool read_all_rpc(sbatteryr_t* battery, sfloorr_t* floor, slightr_t* light, sdistr_t* dist){
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	std::vector<avtuple_t> distances(8);
	bool res = minibot_client->readAllSensors(*battery, distances, *floor, *light);
	dist->count = res ? distances.size() : 8;
	for(uint16_t i = 0; i < 8; ++i){
		if(res && (i < distances.size())) dist->data[i] = distances[i];
		else dist->data[i] = (avtuple_t){-1, -1};
	}
	return res;
}


bool read_batt_rpc(sbatteryr_t* reading){
	if( reading == NULL ) return false;
	reading->voltage = reading->percentage = -1;
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->readBatterySensor(*reading);
}


bool read_dist_rpc(sdistr_t* dist){
	printf("read_dist_rpc\n");
	if( dist == NULL ) return false;
	*dist = (sdistr_t){0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	std::vector<avtuple_t> distances(12);
	bool res = minibot_client->readDistanceSensors(distances);
	dist->count = res ? distances.size() : 12;
	printf("dist->count = %lu\n", distances.size());
	printf("dist->data =");
	for(uint16_t i = 0; i < 12; ++i){
		if(res && (i < distances.size())) dist->data[i] = distances[i];
		else dist->data[i] = (avtuple_t){-1, -1};
		printf(" (%f, %f)", dist->data[i].angle, dist->data[i].value);
	}
	printf("\n");
	return res;
}


bool read_light_rpc(slightr_t* reading){
	if( reading == NULL ) return false;
	*reading = (slightr_t){-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->readLightSensors(*reading);
}


bool read_floor_rpc(sfloorr_t* reading){
	*reading = (sfloorr_t){-1,-1,-1,-1};
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->readFloorSensors(*reading);
}


bool minibot_connect(const std::string& address, uint16_t port, int retries, bool auto_reconnect){
	minibot_cli_port = port;
	minibot_cli_addr = address;
	minibot_cli_autoreconnect = auto_reconnect;

	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	if ( minibot_client == nullptr ){
		minibot_client = MinibotClient::make_shared();
		minibot_client->addConnectedHandler( &minibot_connected_handler );
		minibot_client->addDisconnectedHandler( &minibot_disconnected_handler );
	}

	fprintf(stderr, "Connecting to Minibot on %s:%u.", address.c_str(), port);
	if(!minibot_client->connect(address, port))
		fprintf(stderr, "Could not connect to Minibot on %s:%u. Retrying", address.c_str(), port);
	while(!minibot_client->connected() && (retries-- != 0)){
		fprintf(stderr, ".");
		if(minibot_client->connect(address, port)) break;
		std::this_thread::sleep_for( 1s );
		// fprintf(stderr, minibot_cli_autoreconnect ? "Retrying...\n" : "\n");
	}
	fprintf(stderr, "\n");
	// if(minibot_cli_autoreconnect)
	// 	autoreconnect_thread = std::make_shared<std::thread>( autoreconnect_task );
	return minibot_client->connected();
}


/* ** *****************************************************************
* Local function definitions
* *** ****************************************************************/
bool minibot_connected(){
	// std::lock_guard<std::mutex> lock(minibot_client_mtx);
	return (minibot_client != nullptr) && minibot_client->connected();
}

void autoreconnect_task(){
	while(minibot_cli_autoreconnect && (minibot_client != nullptr)){
		std::this_thread::sleep_for( 3s );
		// std::lock_guard<std::mutex> lock(minibot_client_mtx);
		if( minibot_client->connect(minibot_cli_addr, minibot_cli_port) ) break;
	}
	autoreconnect_thread = nullptr;
}

void minibot_connected_handler(const MinibotClientPtr& client){
	fprintf(stderr, "Connected to Minibot server on %s:%d\n", minibot_cli_addr.c_str(), minibot_cli_port);
}


void minibot_disconnected_handler(const MinibotClientPtr& client){
	fprintf(stderr, "Disconnected from Minibot server on %s:%d\n", minibot_cli_addr.c_str(), minibot_cli_port);
	// if( minibot_cli_autoreconnect && (autoreconnect_thread == nullptr) )
	// 	autoreconnect_thread = std::make_shared<std::thread>( autoreconnect_task );
}