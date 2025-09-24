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

        template<typename T>
        T* getComponent(EntityId entityId) {
            Record* record = mArchetypeManager->getRecord(entityId);
            if (!record || !record->archetype) {
                return nullptr;
            }

            Archetype* archetype = record->archetype;
            auto componentIt = std::find(archetype->type.begin(), archetype->type.end(), ComponentIdTrait<T>::id);
            if (componentIt == archetype->type.end()) {
                // LOG_INFO("Component ID = ", static_cast<u32>(componentIt)," not present in this archetype.");
                return nullptr;
            }
            usize colIdx = std::distance(archetype->type.begin(), componentIt);

            auto* col = static_cast<Column<T>*>(archetype->columns[colIdx]);
            if (record->row >= col->elements.size()) {
                return nullptr;
            }
            return &col->elements[record->row];
        }

        void moveEntity(Archetype* sourceArchetype, Archetype* destinationArchetype, usize row) {
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null.");
                return;
            }

            if (!destinationArchetype) {
                LOG_WARN("Destination archetype pointer is null.");
                return;
            }

            // TODO: Insert a new row into the destination archetype
            // TODO: Move overlapping components over to the destination archetype
            // TODO: Remove the entity from the current archetype
        }

        void addComponent(EntityId entityId, ComponentId componentId) {
            Record& record = mArchetypeManager->createRecord(entityId);
            usize recordRow = record.row;

            Archetype* sourceArchetype = record.archetype;
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null. ");
                return;
            }

            mArchetypeManager->setupArchetypeEdges(sourceArchetype);

            Archetype* destinationArchetype = nullptr;
            auto edgeIt = sourceArchetype->edges.find(componentId);
            if (edgeIt != sourceArchetype->edges.end()) {
                destinationArchetype = edgeIt->second.add;
            }

            if (!destinationArchetype) {
                LOG_INFO("Destination archetype pointer is null. Creating new destination archetype and setting it as 'add' edge of source archetype.");
                destinationArchetype = mArchetypeManager->createArchetype();

                std::vector<ComponentId> newType = sourceArchetype->type;
                if (std::find(newType.begin(), newType.end(), componentId) == newType.end()) {
                    newType.push_back(componentId);
                }

                mArchetypeManager->updateArchetypeTypeAndColumns(destinationArchetype, newType);
                mArchetypeManager->updateArchetypeIndex(destinationArchetype);

                mArchetypeManager->setupArchetypeEdges(destinationArchetype);

                // TODO : check if correct
                ArchetypeEdge edge;
                edge.add = destinationArchetype;
                sourceArchetype->edges.insert({componentId, edge});
            }

            moveEntity(sourceArchetype, destinationArchetype, recordRow);
        }

        void removeComponent(EntityId entityId, ComponentId componentId) {
            Record& record = mArchetypeManager->createRecord(entityId);
            usize recordRow = record.row;

            Archetype* sourceArchetype = record.archetype;
            if (!sourceArchetype) {
                LOG_WARN("Source archetype pointer is null.");
                return;
            }

            mArchetypeManager->setupArchetypeEdges(sourceArchetype);

            Archetype* destinationArchetype = nullptr;
            auto edgeIt = sourceArchetype->edges.find(componentId);
            if (edgeIt != sourceArchetype->edges.end()) {
                destinationArchetype = edgeIt->second.remove;
            }

            if (!destinationArchetype) {
                LOG_INFO("Destination archetype pointer is null. Create new destination archetype and set it to 'remove' edge of source archetype.");
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

                // TODO : check if correct
                ArchetypeEdge edge;
                edge.remove = destinationArchetype;
                sourceArchetype->edges.insert({componentId, edge});
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