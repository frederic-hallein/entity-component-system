#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "components.hpp"

using EntityId = u64;
using ComponentId = u64;

namespace ecs {
    class ComponentManager {
    public:
        ComponentManager() = default;

        void addComponent() {

        }

        bool hasComponent(EntityId entity, ComponentId component) {
            return false;
        }




    private:

    };
}

#endif