#pragma once
#include <mutex>

extern std::mutex data_mutex;
extern float data[1280 * 720 * 3];
extern bool terminal_flag;