#include <mutex>
#include <chrono>
#include <thread>

#include "minibot.h"
#include "minibot/client.h"

using MinibotClient    = minibot::client::MinibotClient;
using MinibotClientPtr = minibot::client::MinibotClientPtr;

using namespace std::literals::chrono_literals;

/* ** *****************************************************************
* File-scope global variables
* *** ****************************************************************/
static std::mutex minibot_client_mtx;
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
bool minibot_connected();
void autoreconnect_task();


/* ** *****************************************************************
* Function definitions
* *** ****************************************************************/
bool mvrobot_rpc(float dist, float angle, float *dxc, float *dyc, float *dac){
	std::unique_lock<std::mutex> lock(minibot_client_mtx);
	if( !minibot_connected() ) return false;
	return minibot_client->mv(dist, angle, *dxc, *dyc, *dac);
}


bool minibot_connect(const std::string& address, uint16_t port, bool auto_reconnect){
	minibot_cli_port = port;
	minibot_cli_addr = address;
	minibot_cli_autoreconnect = auto_reconnect;

	std::unique_lock<std::mutex> lock(minibot_client_mtx);
	if ( minibot_client == nullptr ){
		minibot_client = MinibotClient::make_shared();
		minibot_client->addConnectedHandler( &minibot_connected_handler );
		minibot_client->addDisconnectedHandler( &minibot_disconnected_handler );
	}

	if(!minibot_client->connect(address, port)){
		fprintf(stderr, "Could not connect to Minibot on %s:%u.", address.c_str(), port);
		fprintf(stderr, minibot_cli_autoreconnect ? "Retrying...\n" : "\n");
		if(minibot_cli_autoreconnect)
			autoreconnect_thread = std::make_shared<std::thread>( autoreconnect_task );
	}
	return minibot_client->connected();
}


/* ** *****************************************************************
* Local function definitions
* *** ****************************************************************/
bool minibot_connected(){
	std::unique_lock<std::mutex> lock(minibot_client_mtx);
	return minibot_client == nullptr;
}

void autoreconnect_task(){
	while(minibot_cli_autoreconnect && (minibot_client != nullptr)){
		std::this_thread::sleep_for( 3s );
		std::unique_lock<std::mutex> lock(minibot_client_mtx);
		if( minibot_client->connect(minibot_cli_addr, minibot_cli_port) ) break;
	}
	autoreconnect_thread = nullptr;
}

void minibot_connected_handler(const MinibotClientPtr& client){
	printf("Connected to Minibot server on %s:%d\n", minibot_cli_addr.c_str(), minibot_cli_port);
}


void minibot_disconnected_handler(const MinibotClientPtr& client){
	printf("Disconnected from Minibot server on %s:%d\n", minibot_cli_addr.c_str(), minibot_cli_port);
	if( minibot_cli_autoreconnect && (autoreconnect_thread == nullptr) )
		autoreconnect_thread = std::make_shared<std::thread>( autoreconnect_task );
}