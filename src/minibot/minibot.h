#pragma once
#include <string>
#include <cstdint>

bool minibot_connect(const std::string& address="127.0.0.1", uint16_t port=9000, bool auto_reconnect=true);
bool minibot_disconnect();
bool mvrobot_rpc(float dist, float angle, float *dxc, float *dyc, float *dac);
