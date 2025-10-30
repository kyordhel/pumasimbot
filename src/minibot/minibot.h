#pragma once
#include <string>
#include <cstdint>
#include <minibot/hal.h>


struct {
	uint16_t            count;
	minibot::avtuple_t  data[8];
}typedef sdistr_t;

// Order battery, floor, light, distance
// HDR + 4×4 + 8×4 + 2+n×4 = HDR + 50+n×4


bool minibot_connect(const std::string& address="192.168.1.1", uint16_t port=9000, int retries=-1, bool auto_reconnect=true);
bool minibot_disconnect();
bool minibot_connected();
bool mvrobot_rpc(float dist, float angle, float *dxc, float *dyc, float *dac);

bool robot_set_pwm_rpc(float left, float right, float front, float back);
bool robot_set_speed_rpc(float left, float right, float front, float back);
bool read_all_rpc(minibot::sbatteryr_t* batt, minibot::sfloorr_t* floor, minibot::slightr_t* light, sdistr_t* dist);
bool read_batt_rpc(minibot::sbatteryr_t* reading);
bool read_dist_rpc(sdistr_t* reading);
bool read_light_rpc(minibot::slightr_t* reading);
bool read_floor_rpc(minibot::sfloorr_t* reading);
