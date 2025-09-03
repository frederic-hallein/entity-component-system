#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "pch.hpp"

namespace ecs {
    struct Entity {
        u64 id;
    };

    class EntityManager {
    public:
        EntityManager() = default;

        void createEntity() {
            Entity e{mEntityId++};
            mEntities.push_back(e);
        };

        void createEntities(usize amount) {
            for (u64 i = 0; i < amount; ++i) {
                createEntity();
            }
        }

        void deleteEntity(const Entity& entity) {
            auto it = std::find_if(mEntities.begin(), mEntities.end(),
                [entity](const Entity& e) { return e.id == entity.id; });

            if (it != mEntities.end()) {
                std::iter_swap(it, mEntities.end() - 1);
                mEntities.pop_back();
            }

            LOG_INFO("Deleted entity ID = ", entity.id);
        }

        const std::vector<Entity>& getAllEntities() const {
            return mEntities;
        }

    private:
        u64 mEntityId = 0;
        std::vector<Entity> mEntities;
    };
}

#endif