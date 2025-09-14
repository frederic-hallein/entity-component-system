#ifndef WORLD_HPP
#define WORLD_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "archetypeManager.hpp"
// #include "components.hpp"

static constexpr u32 MAX_ENTITIES = 10;

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

        // void moveEntity(Archetype* currentArchetype, Archetype* nexArchetype, usize row) {
        // }

        void addComponent(EntityId entityId, ComponentId componentId) {
            Record& record = mArchetypeManager->getOrCreateRecord(entityId);
            ArchetypeMap& archetypeMap = mArchetypeManager->getOrCreateArchetypeMap(componentId);
            ArchetypeRecord& archetypeRecord = mArchetypeManager->getOrCreateArchetypeRecord(archetypeMap, record.archetype->id);

            mArchetypeManager->updateArchetypeTypeAndColumns(record.archetype, componentId);
            mArchetypeManager->setupArchetypeEdges(record.archetype);
            mArchetypeManager->updateArchetypeIndex(record.archetype);

            // move entity (not implemented)
        }

        EntityManager* getEntityManager() {
            return mEntityManager.get();
        }

        ArchetypeManager* getArchetypeManager() {
            return mArchetypeManager.get();
        }

    private:
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<ArchetypeManager> mArchetypeManager;
    };
}

#endif