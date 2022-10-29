#include "nemoapp.h"

namespace nemo
{
    // Protected methods for NemoApp
    void NemoApp::_setup()
    {
        // Setup the components
        for (auto &component : __components)
            component->setup();
    };

    void NemoApp::_loop()
    {
        // Loop the components
        for (auto &component : __components)
            component->loop();
    };

    // Public methods for NemoApp
    void NemoApp::register_component(Component &component)
    {
        __components.push_back(&component);
    }

    // Initialize static variables
    std::unordered_map<std::string, Entity> NemoApp::entities;
};