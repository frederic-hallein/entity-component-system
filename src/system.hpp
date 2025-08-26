#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "pch.hpp"

namespace ecs {

    // Forward declarations
    template<u64 MaxEntities>
    class ComponentManager;

    // Forward declarations
    template<u64 MaxEntities>
    class SystemManager;

    template<u64 MaxEntities>
    class System {
    public:
        virtual ~System() = default;

        virtual void update(f32 deltaTime) = 0;
        virtual void init() {}
        virtual void shutdown() {}

    protected:
        template<typename... Components>
        bool entityHasComponents(u64 entityId) const {
            return (mComponentManager->template hasComponent<Components>(entityId) && ...);
        }

        void setComponentManager(ComponentManager<MaxEntities>* componentManager) {
            mComponentManager = componentManager;
        }

        ComponentManager<MaxEntities>* mComponentManager = nullptr;

        friend class SystemManager<MaxEntities>;
    };

}

#endif