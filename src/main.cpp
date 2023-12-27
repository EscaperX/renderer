#include "gl/application.hpp"
#include "gl/window.hpp"
#include <string>
#include <iostream>

int main()
{
    cc::Application *app = new cc::Application{};
    std::string window_title = "Test";
    cc::Window *window = new cc::Window{window_title, app};

    window->mainloop();

    delete app;
    delete window;
}
