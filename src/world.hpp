#ifndef WORLD_HPP
#define WORLD_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "archetypeManager.hpp"

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
            if (!record) {
                LOG_WARN("Record pointer in getComponent is null.");
                return nullptr;
            }

            Archetype* archetype = record->archetype;
            if (!archetype) {
                LOG_WARN("Archetype pointer in getComponent is null.");
                return nullptr;
            }

            auto componentIt = std::find(archetype->type.begin(), archetype->type.end(), ComponentIdTrait<T>::id);
            if (componentIt == archetype->type.end()) {
                LOG_INFO("Component ID = ", static_cast<u32>(ComponentIdTrait<T>::id)," not present in this Archetype.");
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
            if (!sourceArchetype || !destinationArchetype) {
                LOG_WARN("Source Archetype pointer in moveEntity is null.");
                return;
            }

            if (!destinationArchetype) {
                LOG_WARN("Destination Archetype pointer in moveEntity is null.");
                return;
            }

            // Insert a new row into the destination archetype
            for (usize i = 0; i < destinationArchetype->type.size(); ++i) {
                ComponentId cid = destinationArchetype->type[i];
                auto srcIt = std::find(sourceArchetype->type.begin(), sourceArchetype->type.end(), cid);
                if (srcIt != sourceArchetype->type.end()) {
                    usize srcIdx = std::distance(sourceArchetype->type.begin(), srcIt);
                    destinationArchetype->columns[i]->moveElementFrom(sourceArchetype->columns[srcIdx], row);
                } else {
                    destinationArchetype->columns[i]->moveElementFrom(nullptr, 0);
                }
            }

            // Find the entity being moved (by matching archetype and row)
            EntityId movedEntity = 0;
            for (const auto& [entityId, record] : mArchetypeManager->getEntityIndex()) {
                if (record.archetype == sourceArchetype && record.row == row) {
                    movedEntity = entityId;
                    break;
                }
            }

            // Remove the entity from the source archetype
            usize lastRow = 0;
            if (!sourceArchetype->columns.empty()) {
                lastRow = sourceArchetype->columns[0]->size() - 1;
            }

            for (auto* col : sourceArchetype->columns) {
                col->removeElement(row);
            }

            // Update the moved entity's record to point to the new archetype and row
            if (movedEntity != 0) {
                auto& record = mArchetypeManager->getEntityIndex()[movedEntity];
                record.archetype = destinationArchetype;
                if (!destinationArchetype->columns.empty()) {
                    record.row = destinationArchetype->columns[0]->size() - 1; // last row
                } else {
                    record.row = 0;
                }
            }

            // If swap-and-pop was used, update the record of the entity that was swapped in the source archetype
            if (row < lastRow) {
                for (auto& [entityId, record] : mArchetypeManager->getEntityIndex()) {
                    if (record.archetype == sourceArchetype && record.row == lastRow) {
                        record.row = row;
                        break;
                    }
                }
            }
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

            LOG_INFO("Archetype type size: ", sourceArchetype->type.size());

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