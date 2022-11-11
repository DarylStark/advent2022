#ifndef __EVENTBUS_H__
#define __EVENTBUS_H__

#include <unordered_map>
#include <functional>

namespace dsl
{
    namespace event_bus
    {
        typedef std::function<void(int, int)> subscription_function;

        class EventBus
        {
        private:
            std::unordered_map<std::string, std::unordered_map<std::string, subscription_function>> __event_handlers;

        public:
            bool subscribe(const std::string event, const std::string name, subscription_function function);
            bool unsubscribe(const std::string event, const std::string name);
            void raise(const std::string event);
        };
    };
};

#endif /* __EVENTBUS_H__ */