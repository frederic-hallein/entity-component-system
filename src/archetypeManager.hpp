#ifndef ARCHETYPE_MANAGER_HPP
#define ARCHETYPE_MANAGER_HPP

// #include "pch.hpp"
// // #include "world.hpp"
// #include "components.hpp"

// using ArchetypeId = u64;
// using Type = std::vector<ComponentId>;
// using ArchetypeSet = std::unordered_set<ArchetypeId>;
// using Column = vector<T>;

// namespace ecs {
//     struct Archetype {
//         ArchetypeId id;
//         Type type;
//         std::vector<Column> components;
//         std::unordered_map<ComponentId, ArchetypeEdge> edges;
//     };

//     struct ArchetypeEdge {
//         Archetype* add;
//         Archetype* remove;
//     };

//     struct ArchetypeRecord {
//         usize column;
//     };

//     using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;


//     class ArchetypeManager {
//     public:
//         void createArchetype() {

//         }

//         bool hasComponent(Entity entity, u32 component) {
//             return false;
//         }

//         void deleteArchetype() {

//         }

//     private:

//         std::unordered_map<Type, Archetype> archetypeIndex;
//         std::unordered_map<Entity, Archetype*> entityIndex;
//         std::unordered_map<ComponentId, ArchetypeSet> componentIndex;

//     };
// }

#endif