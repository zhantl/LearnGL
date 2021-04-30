#include "event_dispatch.hpp"

EventDispatch *EventDispatch::eventDispatch = nullptr;

Event::Event(const EventType &eventType)
    : m_type(eventType), m_args(nullptr)
{
}

Event::~Event()
{
}

EventListener::EventListener(const EventType &eventType, const CallBack &callBack)
    : m_type(eventType), m_callBack(callBack)
{
}

EventDispatch::EventDispatch()
{
}

EventDispatch::~EventDispatch()
{
    for (auto iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        auto listeners = *iter->second;
        for (auto ls : listeners)
        {
            delete ls;
        }
        listeners.clear();
    }
    m_listeners.clear();
}

EventDispatch *EventDispatch::getInstance()
{
    if (!eventDispatch)
    {
        eventDispatch = new EventDispatch();
    }
    return eventDispatch;
}

void EventDispatch::dispatchEevent(const Event &event)
{
    auto iter = m_listeners.find(event.m_type);

    if (iter != m_listeners.end())
    {
        for (auto listner : *iter->second)
        {
            listner->m_callBack(event);
        }
    }
}

void EventDispatch::addEventListener(EventListener *listener)
{
    std::vector<EventListener *> *listeners = nullptr;
    auto iter = m_listeners.find(listener->m_type);
    if (iter == m_listeners.end())
    {
        listeners = new std::vector<EventListener *>();
        m_listeners.emplace(listener->m_type, listeners);
    }
    else
    {
        listeners = iter->second;
    }
    listeners->push_back(listener);
}

void EventDispatch::removeEventListener(EventListener *listener)
{
    auto iter = m_listeners.find(listener->m_type);
    if (iter != m_listeners.end())
    {
        auto listeners = *iter->second;
        for (auto lt = listeners.begin(); lt != listeners.end(); ++lt)
        {
            if (*lt == listener)
            {
                listeners.erase(lt);
                delete listener;
                break;
            }
        }
    }
}