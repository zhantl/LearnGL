#include <glm/glm.hpp>
#include <graphics/window.hpp>
#include "event_dispatch.hpp"

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

Window *Window::instatnce = nullptr;

Window* Window::getInstance()
{
    if (!instatnce)
    {
        instatnce = new Window;
    }
    return instatnce;
}

Window::Window()
    :m_x(0.), m_y(0.)
{
    for (size_t i = 0; i < MAX_KEY_CODE; i++)
    {
        m_keys[i] = false;
    }
    for (size_t i = 0; i < MAX_MOUSE_CODE; i++)
    {
        m_mouse_button[i] = false;
    }
}

Window::~Window()
{
    std::cout << "window destroy!" << std::endl;
    glfwTerminate();
}

bool Window::initWindow(char const *title, int width, int height)
{
    m_width = width;
    m_height = height;
    m_title = title;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, resize_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        glfwTerminate();
        return false;
    }
    std::cout << "gl version = " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void Window::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::update() const
{
    glCheckError();
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

bool Window::closed() const
{
    return glfwWindowShouldClose(m_window);
}

bool Window::isKeyPress(unsigned int key) const
{
    if (key > MAX_KEY_CODE)
        return false;
    return m_keys[key];
}

bool Window::isMouseButtonPress(unsigned int btn) const
{
    if (btn > MAX_MOUSE_CODE)
        return false;
    return m_mouse_button[btn];
}

void Window::getCursorPosition(double &x, double &y) const
{
    x = m_x;
    y = m_y;
}

void resize_callback(GLFWwindow *window, int width, int height)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_width = width;
    win->m_height = height;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_keys[key] = action != GLFW_RELEASE;

    Event event(EventType::EVNET_KEYBOARD);
    event.setArgs(static_cast<void *>(&key));
    EventDispatch::getInstance()->dispatchEevent(event);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_x = xpos;
    win->m_y = ypos;

    Event event(EventType::EVENT_TOUCH);
    auto pos = new glm::vec2(xpos, ypos);
    event.setArgs(static_cast<void *>(pos));
    EventDispatch::getInstance()->dispatchEevent(event);
    delete pos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_mouse_button[button] = action != GLFW_RELEASE;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Event event(EventType::EVENT_SCROLL);
    auto offset = new glm::vec2(xoffset, yoffset);
    event.setArgs(static_cast<void *>(offset));
    EventDispatch::getInstance()->dispatchEevent(event);
    delete offset;
}
