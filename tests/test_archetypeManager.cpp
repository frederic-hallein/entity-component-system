#include <gtest/gtest.h>
#include "archetypeManager.hpp"

using namespace ecs;

TEST(ArchetypeManagerTest, InitEmptyArchetype) {
    /* -- arrange -- */
    ArchetypeManager manager;

    /* -- act -- */
    auto& archetypeIndex = manager.getArchetypeIndex();

    /* -- assert -- */
    ASSERT_FALSE(archetypeIndex.empty());
    bool foundEmpty = false;
    for (const auto& [typeHash, archetypePtr] : archetypeIndex) {
        ASSERT_NE(archetypePtr, nullptr);
        if (archetypePtr->id == 0 && archetypePtr->type.empty()) {
            foundEmpty = true;
            break;
        }
    }
    EXPECT_TRUE(foundEmpty);
}

TEST(ArchetypeManagerTest, GetOrCreateRecord) {
    /* -- arrange -- */
    ArchetypeManager manager;
    EntityId entityId = 42;

    /* -- act -- */
    Record& firstRecord  = manager.getOrCreateRecord(entityId);
    Record& secondRecord = manager.getOrCreateRecord(entityId);

    /* -- assert -- */
    ASSERT_EQ(&firstRecord, &secondRecord);
}

TEST(ArchetypeManagerTest, GetOrCreateArchetypeMap) {
    /* -- arrange -- */
    ArchetypeManager manager;
    ComponentId componentId = 7;

    /* -- act -- */
    ArchetypeMap& firstMap  = manager.getOrCreateArchetypeMap(componentId);
    ArchetypeMap& secondMap = manager.getOrCreateArchetypeMap(componentId);

    /* -- assert -- */
    ASSERT_EQ(&firstMap, &secondMap);
}

TEST(ArchetypeManagerTest, GetOrCreateArchetypeRecord) {
    /* -- arrange -- */
    ArchetypeManager manager;
    ComponentId componentId = 7;
    ArchetypeId archetypeId = 3;
    ArchetypeMap& archetypeMap = manager.getOrCreateArchetypeMap(componentId);

    /* -- act -- */
    ArchetypeRecord& firstRecord  = manager.getOrCreateArchetypeRecord(archetypeMap, archetypeId);
    ArchetypeRecord& secondRecord = manager.getOrCreateArchetypeRecord(archetypeMap, archetypeId);

    /* -- assert -- */
    ASSERT_EQ(&firstRecord, &secondRecord);
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns) {
    /* -- arrange -- */
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    ComponentId componentId = 1;

    /* -- act -- */
    manager.updateArchetypeTypeAndColumns(archetype, componentId);

    /* -- assert -- */
    ASSERT_EQ(archetype->type.size(), 1);
    EXPECT_EQ(archetype->type[0], componentId);
    ASSERT_EQ(archetype->columns.size(), 1);
}

TEST(ArchetypeManagerTest, UpdateArchetypeIndex) {
    /* -- arrange -- */
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    ComponentId componentId = 2;
    manager.updateArchetypeTypeAndColumns(archetype, componentId);

    /* -- act -- */
    manager.updateArchetypeIndex(archetype);

    /* -- assert -- */
    TypeHash hash = manager.computeTypeHash(archetype->type);
    auto& archetypeIndex = manager.getArchetypeIndex();
    ASSERT_TRUE(archetypeIndex.count(hash));
    EXPECT_EQ(archetypeIndex[hash], archetype);
}

TEST(ArchetypeManagerTest, SetupArchetypeEdges_SingleComponent) {
    /* -- arrange -- */
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    ComponentId componentId = 8;
    manager.updateArchetypeTypeAndColumns(archetype, componentId);

    /* -- act -- */
    manager.setupArchetypeEdges(archetype);

    /* -- assert -- */
    ASSERT_TRUE(archetype->edges.count(componentId));

    Type emptyType;
    TypeHash emptyHash = manager.computeTypeHash(emptyType);
    Archetype* emptyArchetype = manager.getArchetypeIndex()[emptyHash];
    ASSERT_NE(emptyArchetype, nullptr);

    ASSERT_TRUE(archetype->edges.count(componentId));
    EXPECT_EQ(archetype->edges[componentId].remove, emptyArchetype);

    ASSERT_TRUE(emptyArchetype->edges.count(componentId));
    EXPECT_EQ(emptyArchetype->edges[componentId].add, archetype);
}

TEST(ArchetypeManagerTest, SetupArchetypeEdges_MultiComponent) {
    /* -- arrange -- */
    ArchetypeManager manager;

    Type typeA = {1, 3};
    std::sort(typeA.begin(), typeA.end());
    Archetype* archetypeA = manager.createArchetype();
    for (ComponentId cid : typeA) {
        manager.updateArchetypeTypeAndColumns(archetypeA, cid);
    }
    manager.updateArchetypeIndex(archetypeA);

    Type typeB = {1, 3, 2};
    std::sort(typeB.begin(), typeB.end());
    Archetype* archetypeB = manager.createArchetype();
    for (ComponentId cid : typeB) {
        manager.updateArchetypeTypeAndColumns(archetypeB, cid);
    }
    manager.updateArchetypeIndex(archetypeB);

    /* -- act -- */
    manager.setupArchetypeEdges(archetypeB);

    TypeHash hashA = manager.computeTypeHash(typeA);
    TypeHash hashB = manager.computeTypeHash(typeB);

    Archetype* foundA = manager.getArchetypeIndex()[hashA];
    Archetype* foundB = manager.getArchetypeIndex()[hashB];

    /* -- assert -- */
    ASSERT_NE(foundA, nullptr);
    ASSERT_NE(foundB, nullptr);

    // Check that archetypeA's edges for key=2 point to archetypeB (add)
    ASSERT_TRUE(foundA->edges.count(2));
    EXPECT_EQ(foundA->edges[2].add, foundB);
    EXPECT_EQ(foundA->edges[2].remove, nullptr);

    // Check that archetypeB's edges for key=2 point to archetypeA (remove)
    ASSERT_TRUE(foundB->edges.count(2));
    EXPECT_EQ(foundB->edges[2].add, nullptr);
    EXPECT_EQ(foundB->edges[2].remove, foundA);
}