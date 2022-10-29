#ifndef __APP_H__
#define __APP_H__

#include <list>
#include "component.h"
#include "entity.h"

namespace nemo
{
    class NemoApp
    {
    private:
        std::list<Component *> __components;

    protected:
        virtual void _setup();
        virtual void _loop();

    public:
        static std::unordered_map<std::string, Entity> entities;

        // Methods to manage components
        void register_component(Component &component);

        // Pure virtual methods for subclasses
        virtual void setup() = 0;
        virtual void loop() = 0;
    };
};

#endif // __APP_H__