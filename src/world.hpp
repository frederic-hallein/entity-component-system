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
            Record* recordPtr = mArchetypeManager->getOrCreateRecord(entityId);
            Record& record = *recordPtr;

            ArchetypeMap& archetypeMap = mArchetypeManager->getOrCreateArchetypeMap(componentId);
            ArchetypeRecord& archetypeRecord = mArchetypeManager->getOrCreateArchetypeRecord(archetypeMap, record.archetype->id);

            mArchetypeManager->updateArchetypeTypeAndColumns(record.archetype, componentId);

            TypeHash typeHash = computeTypeHash(record.archetype->type);
            mArchetypeManager->updateArchetypeIndex(typeHash, record.archetype);

            mArchetypeManager->setupArchetypeEdges(record.archetype);

            // move entity (not implemented)
        }

        // // TODO : take into account new default values
        // std::any getComponent(EntityId entityId, ComponentId componentId) {
        //     auto& entityIndex = mArchetypeManager->getEntityIndex();
        //     auto entityIt = entityIndex.find(entityId);
        //     Record record = (entityIt != entityIndex.end()) ? entityIt->second : Record{};
        //     Archetype* archetype = record.archetype;

        //     auto& componentIndex = mArchetypeManager->getComponentIndex();
        //     auto componentIt = componentIndex.find(componentId);
        //     ArchetypeMap archetypeMap = (componentIt != componentIndex.end()) ? componentIt->second : ArchetypeMap{};

        //     if (archetypeMap.count(archetype->id) == 0) {
        //         return nullptr;
        //     }

        //     ArchetypeRecord& archetypeRecord = archetypeMap[archetype->id];
        //     return std::any_cast<Column<std::any>&>(archetype->columns[archetypeRecord.column]).elements[record.row];
        // }

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