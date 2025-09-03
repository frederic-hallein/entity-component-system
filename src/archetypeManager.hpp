#ifndef ARCHETYPE_MANAGER_HPP
#define ARCHETYPE_MANAGER_HPP

#include "pch.hpp"
// #include "world.hpp"
#include "entityManager.hpp"
#include "components.hpp"

using EntityId = u64;
using ComponentId = u64;
using ArchetypeId = u64;
using TypeHash = u64;
using Type = std::vector<ComponentId>;
using ArchetypeSet = std::unordered_set<ArchetypeId>;

// computes hash for Type so it can be used as key for archetypeIndex
TypeHash computeTypeHash(const std::vector<u64>& type) {
    TypeHash h = 0;
    for (u64 x : type)
        h ^= std::hash<u64>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
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
        std::vector<Column<std::any>> components;
        std::unordered_map<ComponentId, ArchetypeEdge> edges;
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

        bool hasComponent(EntityId entity, ComponentId component) {
            return false;
        }

        void deleteArchetype() {

        }

    private:
        std::unordered_map<TypeHash, Archetype> archetypeIndex;
        // std::unordered_map<Type, Archetype> archetypeIndex;
        std::unordered_map<EntityId, Archetype*> entityIndex;
        std::unordered_map<ComponentId, ArchetypeSet> componentIndex;

    };
}

#endif