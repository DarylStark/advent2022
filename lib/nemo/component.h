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
        Component(const std::string &name) : _name(name) {}
        virtual void setup() = 0;
        virtual void loop() = 0;
    };
};

#endif // __COMPONENT_H__