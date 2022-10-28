#ifndef __COMPONENT_H__
#define __COMPONENT_H__

namespace nemo
{
    class Component
    {
    public:
        virtual void setup();
        virtual void loop();
    };
};

#endif // __COMPONENT_H__