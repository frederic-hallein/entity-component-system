#ifndef ARCHETYPE_MANAGER_HPP
#define ARCHETYPE_MANAGER_HPP

#include "pch.hpp"
// #include "world.hpp"
#include "entityManager.hpp"
// #include "components.hpp"
#include "components.hpp"

using EntityId = u32;
using ComponentId = u32;
using ArchetypeId = u32;
using TypeHash = u32;
using Type = std::vector<ComponentId>;
using ArchetypeSet = std::unordered_set<ArchetypeId>;

// computes hash for Type so it can be used as key for archetypeIndex
TypeHash computeTypeHash(const std::vector<u32>& type) {
    TypeHash h = 0;
    for (u32 x : type)
        h ^= std::hash<u32>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2); // Magic Constant (0x9e3779b9) used in hash functions to help distribute values more evenly
    return h;
}

namespace ecs {
    struct Archetype;
    struct ArchetypeEdge {
        Archetype* add;
        Archetype* remove;
    };
    struct Archetype {
        ArchetypeId id;
        Type type;
        // std::vector<Column<std::any>> components;
        std::vector<std::any> columns;
        std::unordered_map<ComponentId, ArchetypeEdge> edges;
    };

    struct Record {
        Archetype* archetype;
        usize row;
    };

    struct ArchetypeRecord {
        usize column;
    };

    using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;


    class ArchetypeManager {
    public:
        ArchetypeManager() = default;

        void createArchetype() {

        }

        void deleteArchetype() {

        }

        const std::unordered_map<EntityId, Record>& getEntityIndex() const {
            return entityIndex;
        }

        const std::unordered_map<ComponentId, ArchetypeMap>& getComponentIndex() const {
            return componentIndex;
        }

    private:
        // std::unordered_map<Type, Archetype> archetypeIndex;
        std::unordered_map<TypeHash, Archetype> archetypeIndex;
        // std::unordered_map<EntityId, Archetype*> entityIndex;
        std::unordered_map<EntityId, Record> entityIndex;
        std::unordered_map<ComponentId, ArchetypeMap> componentIndex;

    };
}

#endif