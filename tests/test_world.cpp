#include <gtest/gtest.h>
#include "world.hpp"

using namespace ecs;

TEST(WorldTest, CreateEntity) {
    /* -- arrange -- */
    World world;

    /* -- act -- */
    world.getEntityManager()->createEntity();

    /* -- assert -- */
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 1);
    EXPECT_EQ(world.getEntityManager()->getAllEntities()[0].id, 0);
}

TEST(WorldTest, CreateEntities) {
    /* -- arrange -- */
    World world;

    /* -- act -- */
    world.getEntityManager()->createEntities(10);

    /* -- assert -- */
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 10);
    for (u64 i = 0; i < 10; ++i) {
        EXPECT_EQ(world.getEntityManager()->getAllEntities()[i].id, i);
    }
}

TEST(WorldTest, DeleteEntity) {
    /* -- arrange -- */
    World world;
    world.getEntityManager()->createEntities(10);
    Entity toDelete{5};

    /* -- act -- */
    world.getEntityManager()->deleteEntity(toDelete);

    /* -- assert -- */
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 9);
    for (const auto& entity : world.getEntityManager()->getAllEntities()) {
        EXPECT_NE(entity.id, toDelete.id);
    }
}

TEST(WorldTest, GetOrCreateRecord) {
    /* -- arrange -- */
    World world;
    EntityId entityId = 42;

    /* -- act -- */
    Record* firstRecord  = world.getArchetypeManager()->getOrCreateRecord(entityId);
    Record* secondRecord = world.getArchetypeManager()->getOrCreateRecord(entityId);

    /* -- assert -- */
    ASSERT_NE(firstRecord, nullptr);
    ASSERT_NE(firstRecord->archetype, nullptr);
    ASSERT_EQ(firstRecord, secondRecord);
    EXPECT_EQ(firstRecord->archetype, secondRecord->archetype);
}

TEST(WorldTest, GetOrCreateArchetypeMap) {
    /* -- arrange -- */
    World world;
    ComponentId componentId = 7;

    /* -- act -- */
    ArchetypeMap& firstMap  = world.getArchetypeManager()->getOrCreateArchetypeMap(componentId);
    ArchetypeMap& secondMap = world.getArchetypeManager()->getOrCreateArchetypeMap(componentId);

    /* -- assert -- */
    ASSERT_EQ(&firstMap, &secondMap);
    EXPECT_TRUE(firstMap.empty());
    ArchetypeId archetypeId = 3;
    ArchetypeRecord record;
    firstMap[archetypeId] = record;
    EXPECT_EQ(secondMap.count(archetypeId), 1);
}

TEST(WorldTest, GetOrCreateArchetypeRecord) {
    /* -- arrange -- */
    World world;
    ComponentId componentId = 7;
    ArchetypeId archetypeId = 3;
    ArchetypeMap& archetypeMap = world.getArchetypeManager()->getOrCreateArchetypeMap(componentId);

    /* -- act -- */
    ArchetypeRecord& firstRecord  = world.getArchetypeManager()->getOrCreateArchetypeRecord(archetypeMap, archetypeId);
    ArchetypeRecord& secondRecord = world.getArchetypeManager()->getOrCreateArchetypeRecord(archetypeMap, archetypeId);

    /* -- assert -- */
    ASSERT_EQ(&firstRecord, &secondRecord);
}


// TEST(WorldTest, AddComponent) {
//     /* -- arrange -- */
//     World world;
//     EntityId entityId = 1;
//     ComponentId componentId = 2;
//     /* -- act -- */
//     /* -- assert -- */
// }

