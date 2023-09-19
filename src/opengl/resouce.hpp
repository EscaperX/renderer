#pragma once
#include <mutex>
#include <condition_variable>
extern std::mutex data_mutex;
extern float data[1280 * 720 * 3];
extern bool terminal_flag, updated;
extern std::condition_variable cv;
