#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "pch.hpp"

namespace ecs {
    using EntityId = u32;

    class EntityManager {
    public:
        EntityManager() = default;

        EntityId createEntity() {
            EntityId id = mEntityId++;
            mEntities.push_back(id);
            LOG_INFO("Created Entity ID = ", id);
            return id;
        }

        void createEntities(usize amount, std::vector<EntityId>& outEntities) {
            outEntities.reserve(outEntities.size() + amount);
            for (usize i = 0; i < amount; ++i) {
                outEntities.push_back(createEntity());
            }
        }

        void deleteEntity(EntityId id) {
            auto it = std::find(mEntities.begin(), mEntities.end(), id);
            if (it != mEntities.end()) {
                std::iter_swap(it, mEntities.end() - 1);
                mEntities.pop_back();
                LOG_INFO("Deleted entity ID = ", id);
            }
        }

        const std::vector<EntityId>& getAllEntities() const {
            return mEntities;
        }

    private:
        EntityId mEntityId = 0;
        std::vector<EntityId> mEntities;
    };
}

#endif