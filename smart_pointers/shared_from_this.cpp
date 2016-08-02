#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class IListener
{
public:
    virtual ~IListener() = default;
    virtual void Process() = 0;
    virtual std::shared_ptr<IListener> GetShared() = 0;
};

class SomeListener : public IListener, public std::enable_shared_from_this<SomeListener>
{
public:
    SomeListener(const std::string& name) : m_name(name) { }
    void Process() override;
    std::shared_ptr<IListener> GetShared() override;

private:
    std::string m_name;
};

class Monitor
{
public:
    void Subscribe(IListener& listener);
    void Run();

private:
    std::vector<std::weak_ptr<IListener>> m_listeners;
};

void SomeListener::Process()
{
    std::cout << "Process from SomeListener: " << m_name << std::endl;
}

std::shared_ptr<IListener> SomeListener::GetShared()
{
    return shared_from_this();
}

void Monitor::Subscribe(IListener& listener)
{
    m_listeners.push_back(listener.GetShared());
}

void Monitor::Run()
{
    auto lPromoteAndProcess = [](std::weak_ptr<IListener>& item)
    {
        if (std::shared_ptr<IListener> l = item.lock())
        {
            l->Process();
        }
        else
        {
            std::cout << "Unable to lock std::weak_ptr" << std::endl;
        }
    };
    std::for_each(m_listeners.begin(), m_listeners.end(), lPromoteAndProcess);
}

int main()
{
    Monitor m;
    auto longLifeListener = std::make_shared<SomeListener>("Long life");
    m.Subscribe(*longLifeListener);

    {
        auto shortLivedListener = std::make_shared<SomeListener>("Short life");
        m.Subscribe(*shortLivedListener);
    }

    try 
    {
        SomeListener l2("Wrong usage");
        m.Subscribe(l2);
    }
    catch (std::bad_weak_ptr& e)
    {
        std::cout << "Wrong usage: not from a shared pointer " << e.what() << std::endl;
    }

    try
    {
        std::unique_ptr<SomeListener> l3(new SomeListener("Unique ptr"));
        m.Subscribe(*l3);
    }
    catch (std::bad_weak_ptr& e)
    {
        std::cout << "Wrong usage: not owned " << e.what() << std::endl;
    }
    
    std::thread t(&Monitor::Run, m);
    t.join();

    return 1;
}
