#include "window.hpp"
#include <thread>
#include <random>

void update_resouce()
{
    static int frame_cnt = 0;
    while (!terminal_flag) {
        while (updated);
        data_mutex.lock();
        frame_cnt ++;
        std::cout << frame_cnt<< std::endl;
        // float r = float(rand()) / RAND_MAX;
        // float g = float(rand()) / RAND_MAX;
        // float b = float(rand()) / RAND_MAX;
        float r = 1.0f, g = 0.0f, b = 0.0f;
        for (int i = 0; i < 1280 * 720 * 3; i += 3) {
            data[i] = r;
            data[i + 1] = g;
            data[i + 2] = b;
        }
        updated = true;
        cv.notify_one();
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
