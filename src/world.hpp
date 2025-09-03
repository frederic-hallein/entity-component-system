#ifndef WORLD_HPP
#define WORLD_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "archetypeManager.hpp"
// #include "components.hpp"

static constexpr u64 MAX_ENTITIES = 10;

namespace ecs {
    class World {
    public:
        World() {
            mArchetypeManager = std::make_unique<ArchetypeManager>();
            mEntityManager = std::make_unique<EntityManager>();
            LOG_INFO("World initialized");
        }

        void createEntities() {
            mEntityManager->createEntities(MAX_ENTITIES);
        }

        EntityManager* getEntityManager() { return mEntityManager.get(); }
        const EntityManager* getEntityManager() const { return mEntityManager.get(); }

    private:
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<ArchetypeManager> mArchetypeManager;
    };
}

#endif