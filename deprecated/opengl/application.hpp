#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.hpp"

class Application
{
public:
    Application(int w = 1280, int h = 720);

    virtual ~Application();

    void run();

    // void start();

    // void stop();

protected:
    // std::thread thread;
    /* window info */
    GLFWwindow *_window = nullptr;
    std::string _windowTitle;
    int _windowWidth = 1280;
    int _windowHeight = 720;
    bool _windowReized = false;

    /* timer for fps */
    std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimeStamp;
    float _deltaTime = 0.0f;

    /* input handler */
    KeyboardInput _keyboardInput;
    MouseInput _mouseInput;

    /* clear color */

    void updateTime();

    /* derived class can override this function to handle input */
    virtual void handleInput() = 0;

    /* derived class can override this function to render a frame */
    virtual void renderFrame() = 0;

    void showFpsInWindowTitle();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    static void cursorMovedCallback(GLFWwindow *window, double xPos, double yPos);

    static void mouseClickedCallback(GLFWwindow *window, int button, int action, int mods);

    static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
