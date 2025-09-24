#ifndef ARCHETYPE_MANAGER_HPP
#define ARCHETYPE_MANAGER_HPP

#include "pch.hpp"
#include "entityManager.hpp"
#include "components.hpp"

namespace ecs {
    using EntityId = u32;
    using ArchetypeId = u32;
    using TypeHash = u32;
    using Type = std::vector<ComponentId>;

    struct Archetype;
    struct ArchetypeEdge {
        Archetype* add = nullptr;
        Archetype* remove = nullptr;
    };
    struct Archetype {
        ArchetypeId id = static_cast<ArchetypeId>(-1);
        Type type = {};
        std::vector<void*> columns = {};
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
        ArchetypeManager() : mNextId(0) {
            initEmptyArchetype();
            LOG_INFO("ArchetypeManager initialized.");
        }

        ~ArchetypeManager() = default;

        Archetype* createArchetype() {
            Archetype archetype;
            archetype.id = mNextId++;
            mArchetypes.push_back(archetype);
            LOG_INFO("Created Archetype with ID = ", archetype.id, ".");
            return &mArchetypes.back();
        }

        Record* getRecord(EntityId entityId) {
            auto entityIt = mEntityIndex.find(entityId);
            if (entityIt != mEntityIndex.end()) {
                LOG_INFO("Found Record for Entity ID = ", entityId, ".");
                return &entityIt->second;
            }

            LOG_INFO("Did not find Record for Entity ID = ", entityId, ".");
            return nullptr;
        }

        Record& createRecord(EntityId entityId) {
            if (Record* record = getRecord(entityId)) {
                return *record;
            }

            LOG_INFO("Create new Record for Entity ID = ", entityId," and set it to the Empty Archetype.");

            TypeHash emptyHash = computeTypeHash();
            Archetype* emptyArchetype = nullptr;
            auto archIt = mArchetypeIndex.find(emptyHash);
            if (archIt != mArchetypeIndex.end()) {
                emptyArchetype = archIt->second;
            }

            Record newRecord;
            newRecord.archetype = emptyArchetype;
            usize row = 0;
            for (const auto& [_, rec] : mEntityIndex) {
                if (rec.archetype == emptyArchetype) {
                    ++row;
                }
            }
            newRecord.row = row;

            auto [it, _] = mEntityIndex.insert({entityId, newRecord});
            return it->second;
        }

        ArchetypeMap* getArchetypeMap(ComponentId componentId) {
            auto componentIt = mComponentIndex.find(componentId);
            if (componentIt != mComponentIndex.end()) {
                LOG_INFO("Found ArchetypeMap for Component ID = ", static_cast<u32>(componentId), ".");
                return &componentIt->second;
            }

            LOG_INFO("Did not find ArchetypeMap for Component ID = ", static_cast<u32>(componentId), ".");
            return nullptr;
        }

        ArchetypeMap& createArchetypeMap(ComponentId componentId) {
            if (ArchetypeMap* archetypeMap = getArchetypeMap(componentId)) {
                return *archetypeMap;
            }

            LOG_INFO("Create new ArchetypeMap for Component ID = ", static_cast<u32>(componentId),".");

            ArchetypeMap newArchetypeMap;
            auto [it, _] = mComponentIndex.insert({componentId, newArchetypeMap});
            return it->second;
        }

        ArchetypeRecord* getArchetypeRecord(ArchetypeMap& archetypeMap, ArchetypeId archetypeId) {
            auto archetypeMapIt = archetypeMap.find(archetypeId);
            if (archetypeMapIt != archetypeMap.end()) {
                LOG_INFO("Found ArchetypeRecord for Archetype ID = ", archetypeId, ".");
                return &archetypeMapIt->second;
            }

            LOG_INFO("Did not find ArchetypeRecord for ArchetypeId ID = ", archetypeId, ".");
            return nullptr;
        }

        ArchetypeRecord& createArchetypeRecord(ArchetypeMap& archetypeMap, ArchetypeId archetypeId) {
            if (ArchetypeRecord* archetypeRecord = getArchetypeRecord(archetypeMap, archetypeId)) {
                return *archetypeRecord;
            }

            LOG_INFO("Create new ArchetypeRecord for Archetype ID = ", archetypeId, ".");

            ArchetypeRecord newArchetypeRecord;
            auto [it, _] = archetypeMap.insert({archetypeId, newArchetypeRecord});
            return it->second;
        }

        void updateArchetypeTypeAndColumns(Archetype* archetype, const std::vector<ComponentId>& componentIds) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            if (componentIds.empty()) {
                LOG_INFO("No components to add to archetype.");
                return;
            }

            // sort type
            std::vector<ComponentId>& type = archetype->type;
            for (ComponentId componentId : componentIds) {
                if (std::find(type.begin(), type.end(), componentId) != type.end()) {
                    LOG_INFO("Archetype already contains Component ID = ", static_cast<u32>(componentId), ". Skip.");
                    continue;
                }

                type.push_back(componentId);
            }
            std::sort(type.begin(), type.end());

            LOG_INFO("Match Archetype Columns with sorted Type.");
            archetype->columns.clear();
            for (ComponentId componentId : type) {
                archetype->columns.push_back(components[static_cast<u32>(componentId)]);
            }
        }

        void setupArchetypeEdges(Archetype* archetype) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            archetype->edges.clear();

            auto& type = archetype->type;
            if (type.size() == 0) {
                LOG_INFO("Archetype has 0 components. Ignore.");
                return;
            }

            if (type.size() == 1) {
                LOG_INFO("Archetype has 1 component. Set 'add' Edge of empty Archetype to current Archetype and 'remove' Edge of current Archetype to empty Archetype.");

                ComponentId componentId = archetype->type[0];
                TypeHash emptyHash = computeTypeHash();
                Archetype* emptyArchetype = mArchetypeIndex.at(emptyHash);

                ArchetypeEdge currentArchetypeEdge;
                currentArchetypeEdge.remove = emptyArchetype;
                auto& currentArchetypeEdges = archetype->edges;
                currentArchetypeEdges.insert({componentId, currentArchetypeEdge});

                ArchetypeEdge emptyArchetypeEdge;
                emptyArchetypeEdge.add = archetype;
                auto& emptyArchetypeEdges = emptyArchetype->edges;
                emptyArchetypeEdges.insert({componentId, emptyArchetypeEdge});

                return;
            }

            LOG_INFO("Archetype has > 1 components. Set 'add' Edge of subset Archetype to current Archetype and 'remove' Edge of current Archetype to subset Archetype.");
            for (const auto& currentId : type) {
                Type subset;
                subset.reserve(type.size() - 1);
                std::copy_if(
                    type.begin(), type.end(),
                    std::back_inserter(subset),
                    [currentId](ComponentId id) { return id != currentId; }
                );

                TypeHash subsetHash = computeTypeHash(subset);
                auto subsetIt = mArchetypeIndex.find(subsetHash);
                if (subsetIt == mArchetypeIndex.end()) {
                    LOG_INFO("Subset Archetype not found. Ignore.");
                    continue;
                }

                Archetype* subsetArchetype = subsetIt->second;
                if (!subsetArchetype) {
                    LOG_WARN("Subset Archetype pointer is null.");
                    return;
                }

                ArchetypeEdge currentArchetypeEdge;
                currentArchetypeEdge.remove = subsetArchetype;
                auto& currentArchetypeEdges = archetype->edges;
                currentArchetypeEdges.insert({currentId, currentArchetypeEdge});

                ArchetypeEdge subsetEdge;
                subsetEdge.add = archetype;
                auto& subsetEdges = subsetArchetype->edges;
                subsetEdges.insert({currentId, subsetEdge});
            }
        }

        // Computes hash for Type so it can be used as key for archetypeIndex (defaults to empty hash)
        // Magic Constant (0x9e3779b9) used in hash functions to help distribute values more evenly
        TypeHash computeTypeHash(const std::vector<ComponentId>& type = {}) {
            TypeHash h = 0;
            for (ComponentId componentId : type)
                h ^= std::hash<u32>{}(static_cast<u32>(componentId)) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }

        void updateArchetypeIndex(Archetype* archetype) {
            if (!archetype) {
                LOG_WARN("Archetype pointer is null.");
                return;
            }

            TypeHash typeHash = computeTypeHash(archetype->type);
            mArchetypeIndex.insert({typeHash, archetype});
        }

        std::unordered_map<EntityId, Record>& getEntityIndex() {
            return mEntityIndex;
        }

        std::unordered_map<ComponentId, ArchetypeMap>& getComponentIndex() {
            return mComponentIndex;
        }

        std::unordered_map<TypeHash, Archetype*>& getArchetypeIndex() {
            return mArchetypeIndex;
        }

    private:
        void initEmptyArchetype() {
            Archetype* emptyArchetype = createArchetype();
            updateArchetypeIndex(emptyArchetype);
        }

    private:
        ArchetypeId mNextId;
        std::vector<Archetype> mArchetypes;

        std::unordered_map<TypeHash, Archetype*> mArchetypeIndex;
        std::unordered_map<EntityId, Record> mEntityIndex;
        std::unordered_map<ComponentId, ArchetypeMap> mComponentIndex;
    };
}

#endif