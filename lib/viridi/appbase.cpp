#include "appbase.h"

namespace viridi
{
    namespace apps
    {
        // Protected methods for AppBase
        void AppBase::_setup()
        {
            // Setup the components
            for (auto &component : __components)
                component->setup();
        };

        void AppBase::_loop()
        {
            // Loop the components
            for (auto &component : __components)
                component->loop();
        };

        // Public methods for AppBase
        void AppBase::register_component(components::Component &component)
        {
            __components.push_back(&component);
        }

        // Initialize static variables
        std::unordered_map<std::string, entity_manager::Entity> AppBase::entities;
    };
};