#include "arduino_apps/appbase.h"

namespace dsl
{
    namespace arduino_apps
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
        void AppBase::register_component(arduino_components::Component &component)
        {
            __components.push_back(&component);
        }
    };
};