#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

namespace nemo
{
    class Component
    {
    protected:
        std::string _name;

    public:
        Component(const std::string &name);
        virtual void setup() = 0;
        virtual void loop() = 0;

        // Important TODO:
        // Make sure there is a method to unsubscribe from all the created
        // entities and to delete the created entities once this element
        // gets out of scope.
    };
};

#endif // __COMPONENT_H__