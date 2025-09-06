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

        std::any getComponent(EntityId entityId, ComponentId componentId) {
            auto& entityIndex = mArchetypeManager->getEntityIndex();
            auto entityIt = entityIndex.find(entityId);
            Record record = (entityIt != entityIndex.end()) ? entityIt->second : Record{};
            Archetype* archetype = record.archetype;

            auto& componentIndex = mArchetypeManager->getComponentIndex();
            auto componentIt = componentIndex.find(componentId);
            ArchetypeMap archetypeMap = (componentIt != componentIndex.end()) ? componentIt->second : ArchetypeMap{};

            if (archetypeMap.count(archetype->id) == 0) {
                return nullptr;
            }

            ArchetypeRecord& archetypeRecord = archetypeMap[archetype->id];
            return std::any_cast<Column<std::any>&>(archetype->columns[archetypeRecord.column]).elements[record.row];
        }

        EntityManager* getEntityManager() { return mEntityManager.get(); }
        const EntityManager* getEntityManager() const { return mEntityManager.get(); }

    private:
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<ArchetypeManager> mArchetypeManager;
    };
}

#endif