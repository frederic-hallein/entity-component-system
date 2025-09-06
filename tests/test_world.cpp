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

TEST(WorldTest, GetComponent) {
    /* -- arrange -- */
    World world;
    EntityId entityId = 0;
    ComponentId componentId = 0;
    // TODO : add component to entity

    /* -- act -- */
    std::any component = world.getComponent(entityId, componentId);

    /* -- assert -- */
    EXPECT_TRUE(component.has_value());
    // TODO : expect component and try casting
}