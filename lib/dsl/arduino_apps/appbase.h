#ifndef __APP_H__
#define __APP_H__

#include <list>
#include "arduino_components/component.h"
#include "entity_manager/entity.h"

namespace dsl
{
    namespace arduino_apps
    {
        class AppBase
        {
        private:
            std::list<arduino_components::Component *> __components;

        protected:
            virtual void _setup();
            virtual void _loop();

        public:
            // Methods to manage components
            void register_component(arduino_components::Component &component);

            // Pure virtual methods for subclasses
            virtual void setup() = 0;
            virtual void loop() = 0;
        };
    }
};

#endif // __APP_H__