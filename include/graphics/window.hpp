#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>

void resize_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

class Window
{
public:
    enum
    {
        MAX_KEY_CODE = 1024,
        MAX_MOUSE_CODE = 8,
    };
    Window();
    ~Window();
    static Window *getInstance();
    bool initWindow(char const *title, int width, int height);

    void update() const;
    void clear() const;
    bool closed() const;

    inline int getWidth() const { return m_width; };
    inline int getHeight() const { return m_height; };
    bool isKeyPress(unsigned int key) const;
    bool isMouseButtonPress(unsigned int btn) const;
    void getCursorPosition(double &x, double &y) const;

private:
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    friend void resize_callback(GLFWwindow *window, int width, int height);
    friend void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    friend void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    friend void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

private:
    GLFWwindow *m_window;
    int m_width, m_height;
    char const *m_title;

    bool m_keys[MAX_KEY_CODE];
    bool m_mouse_button[MAX_MOUSE_CODE];
    double m_x, m_y;

    static Window *instatnce;
};