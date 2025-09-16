#ifndef WORLD_HPP
#define WORLD_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "archetypeManager.hpp"

static constexpr u32 MAX_ENTITIES = 10;

namespace ecs {
    class World {
    public:
        World() {
            mArchetypeManager = std::make_unique<ArchetypeManager>();
            mEntityManager = std::make_unique<EntityManager>();
            LOG_INFO("World initialized");
        }

        // void createEntities() {
        //     mEntityManager->createEntities(MAX_ENTITIES);
        // }

        void moveEntity(Archetype* sourceArchetype, Archetype* destinationArchetype, usize row) {
            // TODO: Insert a new row into the destination archetype
            // TODO: Move overlapping components over to the destination archetype
            // TODO: Remove the entity from the current archetype
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null.");
                return;
            }

            if (!destinationArchetype) {
                LOG_WARN("Destination archetype pointer is null.");
                return;
            }

            for (const auto& cid : destinationArchetype->type) {
                // TODO
            }
        }

        void addComponent(EntityId entityId, ComponentId componentId) {
            Record& record = mArchetypeManager->getOrCreateRecord(entityId);
            usize recordRow = record.row;

            Archetype* sourceArchetype = record.archetype;
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null. ");
                return;
            }

            mArchetypeManager->setupArchetypeEdges(sourceArchetype);
            Archetype* destinationArchetype = sourceArchetype->edges[componentId].add;

            if (!destinationArchetype) {
                LOG_WARN("Destination archetype pointer is null. Create new destination archetype and set it to 'add' edge of source archetype.");
                Archetype* destinationArchetype = mArchetypeManager->createArchetype();

                std::vector<ComponentId> newType = sourceArchetype->type;
                auto it = std::find(newType.begin(), newType.end(), componentId);
                if (it != newType.end()) {
                    newType.push_back(componentId);
                }

                mArchetypeManager->updateArchetypeTypeAndColumns(destinationArchetype, newType);
                mArchetypeManager->updateArchetypeIndex(destinationArchetype);

                mArchetypeManager->setupArchetypeEdges(sourceArchetype);
            }

            moveEntity(sourceArchetype, destinationArchetype, recordRow);
        }

        void removeComponent(EntityId entityId, ComponentId componentId) {
            Record& record = mArchetypeManager->getOrCreateRecord(entityId);
            usize recordRow = record.row;

            Archetype* sourceArchetype = record.archetype;
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null.");
                return;
            }

            mArchetypeManager->setupArchetypeEdges(sourceArchetype);
            Archetype* destinationArchetype = sourceArchetype->edges[componentId].remove;

            if (!destinationArchetype) {
                LOG_WARN("Destination archetype pointer is null. Create new destination archetype and set it to 'remove' edge of source archetype.");
                destinationArchetype = mArchetypeManager->createArchetype();

                std::vector<ComponentId> newType = sourceArchetype->type;
                auto it = std::find(newType.begin(), newType.end(), componentId);
                if (it != newType.end()) {
                    std::iter_swap(it, newType.end() - 1);
                    newType.pop_back();
                }

                mArchetypeManager->updateArchetypeTypeAndColumns(destinationArchetype, newType);
                mArchetypeManager->updateArchetypeIndex(destinationArchetype);

                mArchetypeManager->setupArchetypeEdges(destinationArchetype);
            }

            moveEntity(sourceArchetype, destinationArchetype, recordRow);
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