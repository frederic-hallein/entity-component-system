#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "pch.hpp"
#include "componentManager.hpp"

namespace ecs {
    template<typename T>
    struct Entity {
        T id;
    };

    template<typename T, u64 maxEntities>
    class EntityManager {
    public:
        EntityManager() {
            mEntities.reserve(maxEntities);
            for (T i = 0; i < maxEntities; ++i) {
                mEntities.push_back({static_cast<T>(i)});
            }
            LOG_INFO("Constructed ", maxEntities, " Entities");
        }

        void setComponent(
            Entity<T> entity,
            Component component
        )
        {
            for (usize i = 0; i < mEntities.size(); ++i) {

            }
            // LOG_INFO("Added Component to Entity ID = ", entity.id);
        }

        void setComponents(
            Entity<T> entity,
            const std::vector<Component>& components
        )
        {

            // LOG_INFO("Added ", components.size(), " Components to Entity ID = ", entity.id);
        }

    private:
        std::vector<Entity<T>> mEntities;
        std::map<T, std::vector<Component>> mEntityComponents;
    };
}

#endif