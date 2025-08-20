#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "components.hpp"

namespace ecs {

    template<typename EntityIDType, u64 MaxEntities>
    class ComponentManager {
    public:
        ComponentManager() {
            LOG_INFO("Constructed ComponentManager for ", MaxEntities, " entities");
        }

        template<typename Component>
        void setComponent(
            EntityIDType entityId,
            Component component
        )
        {
            mEntityComponents[entityId].emplace_back(std::any(component));
            LOG_INFO("Added Component to Entity ID = ", entityId);
        }

        template<typename Component>
        void setComponents(
            EntityIDType entityId,
            const std::vector<Component>& components
        )
        {
            for (const auto& comp : components) {
                setComponent(entityId, comp);
            }
        }

    private:
        std::unordered_map<EntityIDType, std::vector<std::any>> mEntityComponents;
    };

}

#endif