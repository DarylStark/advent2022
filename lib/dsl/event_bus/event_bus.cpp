#include "event_bus/event_bus.h"

namespace dsl
{
    namespace event_bus
    {
        bool EventBus::subscribe(const std::string event, const std::string name, subscription_function function)
        {
            const auto [it, success] = __event_handlers[event].insert({name, function});
            return success;
        }

        bool EventBus::unsubscribe(const std::string event, const std::string name)
        {
            uint16_t removed = __event_handlers[event].erase(name);
            return removed > 0;
        }

        void EventBus::raise(const std::string event)
        {
            const auto it = __event_handlers.find(event);
            if (it != __event_handlers.end())
            {
                for (const auto &handler : __event_handlers[event])
                {
                    handler.second(1, 2);
                }
            }
        };
    };
};