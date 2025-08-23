#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "components.hpp"

namespace ecs {

    template<u64 MaxEntities>
    class ComponentManager {
    public:
        template<typename Component>
        void setComponent(
            u64 entityId,
            Component component
        )
        {
            std::type_index componentTypeId = std::type_index(typeid(Component));

            // Check if component already exists
            auto& entityComponents = mEntityComponents[entityId];
            auto componentIt = entityComponents.find(componentTypeId);

            if (componentIt != entityComponents.end()) {
                LOG_WARN("Component ", componentTypeId.name(), " already exists on Entity ", entityId, "! Ignoring new component.");
                return;
            }

            // Component doesn't exist, add it
            entityComponents[componentTypeId] = std::any(component);
            LOG_INFO("Added Component ", componentTypeId.name(), " to Entity ID = ", entityId);
        }

        template<typename Component>
        Component* getComponent(u64 entityId) {
            std::type_index componentTypeId = std::type_index(typeid(Component));

            auto entityIt = mEntityComponents.find(entityId);
            if (entityIt == mEntityComponents.end()) {
                LOG_WARN("Entity ", entityId, " does not exist!");
                return nullptr;
            }

            auto componentIt = entityIt->second.find(componentTypeId);
            if (componentIt == entityIt->second.end()) {
                LOG_WARN("Component ", componentTypeId.name(), " is not found!");
                return nullptr;
            }

            try {
                return std::any_cast<Component>(&componentIt->second);
            } catch (const std::bad_any_cast&) {
                LOG_ERROR("Type mismatch!");
                return nullptr;
            }
        }

        template<typename Component>
        bool hasComponent(u64 entityId) {
            std::type_index componentTypeId = std::type_index(typeid(Component));

            auto entityIt = mEntityComponents.find(entityId);
            if (entityIt == mEntityComponents.end()) {
                return false;
            }

            return entityIt->second.find(componentTypeId) != entityIt->second.end();
        }

    private:
        std::unordered_map<u64, std::unordered_map<std::type_index, std::any>> mEntityComponents;

    };

}

#endif