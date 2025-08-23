#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "pch.hpp"
#include "componentManager.hpp"

namespace ecs {
    struct Entity {
        u64 id;
    };

    template<u64 MaxEntities>
    class EntityManager {
    public:
        EntityManager() {
            mEntities.reserve(MaxEntities);
            for (u64 i = 0; i < MaxEntities; ++i) {
                mEntities.push_back({i});
            }
            LOG_INFO("Constructed ", MaxEntities, " Entities");
        }



    private:
        std::vector<Entity> mEntities;
    };
}

#endif