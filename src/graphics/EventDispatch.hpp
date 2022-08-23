#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

class EventDispatch;

enum class EventType
{
    EVENT_TOUCH = 0,
    EVENT_MOUSE = 1,
    EVNET_KEYBOARD = 2,
    EVENT_SCROLL = 3
};

class Event
{
public:
    explicit Event(const EventType &eventType);
    ~Event();
    void setArgs(void *args) { m_args = args; }
    void* getArgs() const { return m_args; }

private:
    EventType m_type;
    void *m_args;
    friend class EventDispatch;
};

class EventListener
{
public:
    typedef std::function<void(const Event&)> CallBack;
    EventListener(const EventType &eventType, const CallBack& callBack);
private:
    EventType m_type;
    CallBack m_callBack;
    friend class EventDispatch;
};

class EventDispatch
{
public:
    static EventDispatch *getInstance();

    EventDispatch();
    ~EventDispatch();

    void dispatchEevent(const Event &event);
    void addEventListener(EventListener *listener);
    void removeEventListener(EventListener *listener);

private:
    EventDispatch(const EventDispatch &) = delete;
    EventDispatch &operator=(const EventDispatch &) = delete;
private:
    std::unordered_map<EventType, std::vector<EventListener *> *> m_listeners;

    static EventDispatch *eventDispatch;
};