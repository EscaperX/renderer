#include "window.hpp"
#include <thread>
#include <random>

void update_resouce()
{
    while (!terminal_flag)
    {
        data_mutex.lock();
        float r = float(rand()) / RAND_MAX;
        float g = float(rand()) / RAND_MAX;
        float b = float(rand()) / RAND_MAX;
        for (int i = 0; i < 1280 * 720 * 3; i += 3)
        {
            data[i] = r;
            data[i + 1] = g;
            data[i + 2] = b;
        }
        data_mutex.unlock();
    }
}

int main()
{
    // Window w;
    // w.run();
    srand(233);
    Window_Thread w;
    w.start();
    std::thread upd_thread(&update_resouce);

    w.stop();
    upd_thread.join();
}