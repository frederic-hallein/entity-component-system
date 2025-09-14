#ifndef ARCHETYPE_MANAGER_HPP
#define ARCHETYPE_MANAGER_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "components.hpp"

using EntityId = u32;
using ComponentId = u32;
using ArchetypeId = u32;
using TypeHash = u32;
using Type = std::vector<ComponentId>;

namespace ecs {
    struct Archetype;
    struct ArchetypeEdge {
        Archetype* add = nullptr;
        Archetype* remove = nullptr;
    };
    struct Archetype {
        ArchetypeId id = static_cast<ArchetypeId>(-1);
        Type type = {};
        std::vector<std::any> columns = {};
        std::unordered_map<ComponentId, ArchetypeEdge> edges = {};
    };

    struct Record {
        Archetype* archetype = nullptr;
        usize row = static_cast<usize>(-1);
    };

    struct ArchetypeRecord {
        usize column = static_cast<usize>(-1);
    };

    using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;


    class ArchetypeManager {
    public:
        ArchetypeManager() {
            initEmptyArchetype();
            LOG_INFO("ArchetypeManager initialized.");
        }

        ~ArchetypeManager() = default;

        Archetype* createArchetype() {
            static ArchetypeId nextId = 0;
            Archetype archetype;
            archetype.id = nextId++;
            mArchetypes.push_back(archetype);
            return &mArchetypes.back();
        }

        Record& getOrCreateRecord(EntityId entityId) {
            auto& entityIndex = getEntityIndex();
            auto entityIt = entityIndex.find(entityId);
            if (entityIt != entityIndex.end()) {
                return entityIt->second;
            } else {
                Archetype* newArchetype = createArchetype();
                Record newRecord;
                newRecord.archetype = newArchetype;
                newRecord.row++;
                auto [it, inserted] = entityIndex.insert({entityId, newRecord});
                return it->second;
            }
        }

        ArchetypeMap& getOrCreateArchetypeMap(ComponentId componentId) {
            auto& componentIndex = getComponentIndex();
            auto componentIt = componentIndex.find(componentId);
            if (componentIt != componentIndex.end()) {
                return componentIt->second;
            } else {
                ArchetypeMap newArchetypeMap;
                auto [it, inserted] = componentIndex.insert({componentId, newArchetypeMap});
                return it->second;
            }
        }

        ArchetypeRecord& getOrCreateArchetypeRecord(ArchetypeMap& archetypeMap, ArchetypeId archetypeId) {
            auto archetypeIt = archetypeMap.find(archetypeId);
            if (archetypeIt != archetypeMap.end()) {
                return archetypeIt->second;
            } else {
                ArchetypeRecord newArchetypeRecord;
                auto [it, inserted] = archetypeMap.insert({archetypeId, newArchetypeRecord});
                return it->second;
            }
        }

        void updateArchetypeTypeAndColumns(Archetype* archetype, ComponentId componentId) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            // sort type
            std::vector<u32>& type = archetype->type;
            type.push_back(componentId);
            std::sort(type.begin(), type.end());

            // match columns with sorted type
            archetype->columns.clear();
            for (ComponentId cid : type) {
                archetype->columns.push_back(ecs::components[cid]);
            }
        }

        void setupArchetypeEdges(Archetype* archetype) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            auto& type = archetype->type;
            if (type.size() == 0) {
                LOG_INFO("Archetype has 0 components. Ignore.");
                return;
            }

            if (type.size() == 1) {
                LOG_INFO("Archetype has 1 component.\nSet 'remove' edge of current archetype to empty archetype and \nset 'add' edge of empty archetype to current archetype.");

                ComponentId componentId = archetype->type[0];
                TypeHash emptyHash = computeTypeHash();
                Archetype* emptyArchetype = archetypeIndex[emptyHash];

                ArchetypeEdge currentArchetypeEdge;
                currentArchetypeEdge.remove = emptyArchetype;
                auto& currentArchetypeEdges = archetype->edges;
                currentArchetypeEdges[componentId] = currentArchetypeEdge;

                ArchetypeEdge emptyArchetypeEdge;
                emptyArchetypeEdge.add = archetype;
                auto& emptyArchetypeEdges = emptyArchetype->edges;
                emptyArchetypeEdges[componentId] = emptyArchetypeEdge;

                return;
            }

            for (const auto& currentId : type) {
                Type subset;
                subset.reserve(type.size() - 1);
                std::copy_if(
                    type.begin(), type.end(),
                    std::back_inserter(subset),
                    [currentId](ComponentId id) { return id != currentId; }
                );

                TypeHash subsetHash = computeTypeHash(subset);
                auto subsetIt = archetypeIndex.find(subsetHash);
                if (subsetIt == archetypeIndex.end()) {
                    LOG_INFO("Subset archetype not found. Ignore.");
                    continue;
                }

                Archetype* subsetArchetype = subsetIt->second;
                if (!subsetArchetype) {
                    LOG_WARN("Subset archetype pointer is null.");
                    return;
                }

                ArchetypeEdge currentArchetypeEdge;
                currentArchetypeEdge.remove = subsetArchetype;
                auto& currentArchetypeEdges = archetype->edges;
                currentArchetypeEdges[currentId] = currentArchetypeEdge;

                ArchetypeEdge subsetEdge;
                subsetEdge.add = archetype;
                auto& subsetEdges = subsetArchetype->edges;
                subsetEdges[currentId] = subsetEdge;
            }
        }

        // Computes hash for Type so it can be used as key for archetypeIndex (defaults to empty hash)
        // Magic Constant (0x9e3779b9) used in hash functions to help distribute values more evenly
        TypeHash computeTypeHash(const std::vector<u32>& type = {}) {
            TypeHash h = 0;
            for (u32 x : type)
                h ^= std::hash<u32>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }

        void updateArchetypeIndex(Archetype* archetype) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            TypeHash typeHash = computeTypeHash(archetype->type);
            archetypeIndex.insert({typeHash, archetype});
        }

        std::unordered_map<EntityId, Record>& getEntityIndex() {
            return entityIndex;
        }

        std::unordered_map<ComponentId, ArchetypeMap>& getComponentIndex() {
            return componentIndex;
        }

        std::unordered_map<TypeHash, Archetype*>& getArchetypeIndex() {
            return archetypeIndex;
        }

    private:
        void initEmptyArchetype() {
            Archetype* emptyArchetype = createArchetype();
            updateArchetypeIndex(emptyArchetype);
        }

    private:
        std::vector<Archetype> mArchetypes;

        std::unordered_map<EntityId, Record> entityIndex;
        std::unordered_map<ComponentId, ArchetypeMap> componentIndex;
        std::unordered_map<TypeHash, Archetype*> archetypeIndex;
    };
}

#endif