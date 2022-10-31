#ifndef __APP_H__
#define __APP_H__

#include <list>
#include "components/component.h"
#include "entity_manager/entity.h"

namespace viridi
{
    namespace apps
    {
        class AppBase
        {
        private:
            std::list<components::Component *> __components;

        protected:
            virtual void _setup();
            virtual void _loop();

        public:
            static std::unordered_map<std::string, entity_manager::Entity> entities;

            // Methods to manage components
            void register_component(components::Component &component);

            // Pure virtual methods for subclasses
            virtual void setup() = 0;
            virtual void loop() = 0;
        };
    }
};

#endif // __APP_H__