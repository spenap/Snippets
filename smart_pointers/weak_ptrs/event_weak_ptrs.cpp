#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

class Context;

class Event
{
public:
    Event(int id, Context& context) : m_id(id), m_data(new int[Size()]), m_context(context) { }
    Event(const Event& other) : m_id(other.m_id), m_data(new int[Size()]), m_context(other.m_context) { std::cout << "Copying!!" << std::endl; }
    ~Event() { delete[] m_data; m_data = nullptr; }
    void Process() const;
    const int* Data() const { return m_data; }

    static constexpr int Size() { return 1024; }

private:
    int m_id;
    int* m_data;
    Context& m_context;
};

class Context
{
public:
    Context(std::string name) : m_name(name) { }
    void SomeAPI(const Event* ev);
private:
    std::string m_name;
};

void Context::SomeAPI(const Event* ev)
{
    int accu = 0;
    for (int i = 0; i < Event::Size(); ++i)
    {
        accu += ev->Data()[i];
    }
    std::cout << "Acting on the context " << m_name << " from event." << std::endl;
}

void Event::Process() const
{
    std::cout << "Hello from event " << m_id << ". Data is " << (m_data ? "not null" : "null") << std::endl;
    m_context.SomeAPI(this);
}

class EventDispatcher
{
public:
    void Queue(const std::shared_ptr<Event>& event);
    void Run();

private:
    std::deque<std::weak_ptr<const Event>> m_events;
};

void EventDispatcher::Queue(const std::shared_ptr<Event>& event)
{
    m_events.push_back(event);
}

void EventDispatcher::Run()
{
    std::cout << "Processing events" << std::endl;
    auto promoteAndProcess = [](const std::weak_ptr<const Event>& weakPtr)
    {
        if (std::shared_ptr<const Event> ev = weakPtr.lock())
        {
            ev->Process();
        }
        else
        {
            std::cout << "Pointer has expired!" << std::endl;
        }
    };
    std::for_each(m_events.cbegin(), m_events.cend(), promoteAndProcess);
}

int main(int argc, char** argv)
{
    Context context("ctxt");
    auto ev = std::make_shared<Event>(51, context);
    EventDispatcher dispatcher;
    {
        for (int i = 0; i < 50; ++i)
        {
            std::unique_ptr<Event> pEv(new Event(i, context));
            dispatcher.Queue(std::shared_ptr<Event>(std::move(pEv)));
        }
        dispatcher.Queue(ev);	
    }
    std::thread t(&EventDispatcher::Run, dispatcher);
    t.join();
    return 0;
}
