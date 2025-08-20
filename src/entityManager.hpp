#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "pch.hpp"
#include "componentManager.hpp"

namespace ecs {
    template<typename T>
    struct Entity {
        T id;
    };

    template<typename T, u64 MaxEntities>
    class EntityManager {
    public:
        EntityManager() {
            mEntities.reserve(MaxEntities);
            for (T i = 0; i < MaxEntities; ++i) {
                mEntities.push_back({static_cast<T>(i)});
            }
            LOG_INFO("Constructed ", MaxEntities, " Entities");
        }

        // Entity<T> createEntity() {
        //     if (mCurrentEntityIndex < MaxEntities) {
        //         return mEntities[mCurrentEntityIndex++];
        //     }

        //     LOG_ERROR("Entity limit exceeded!");
        //     return Entity<T>{static_cast<T>(-1)};
        // }

    private:
        std::vector<Entity<T>> mEntities;
    };
}

#endif