#include <gtest/gtest.h>
#include "world.hpp"

using namespace ecs;

TEST(WorldTest, GetComponent_EntityWithoutComponents) {
    // Arrange
    World world;
    EntityId entityId = 5;

    // Act
    Position* pos = world.getComponent<Position>(entityId);

    // Assert
    ASSERT_EQ(pos, nullptr);
}

TEST(WorldTest, AddComponent_EntityWithoutComponents) {
    // Arrange
    World world;
    EntityId e1 = 6;

    // Act
    world.addComponent(e1, ComponentId::Position);

    // Assert
    Record* rec = world.getArchetypeManager()->getRecord(e1);
    ASSERT_EQ(rec->archetype->type.size(), 1);
    EXPECT_EQ(rec->archetype->type[0], ComponentId::Position);
}

TEST(WorldTest, AddComponent_EntityWithMultipleComponents) {
    // Arrange
    World world;
    EntityId e1 = 8;

    // Act
    world.addComponent(e1, ComponentId::Position);
    world.addComponent(e1, ComponentId::Velocity);

    // Assert
    Record* rec = world.getArchetypeManager()->getRecord(e1);
    ASSERT_NE(rec, nullptr);
    ASSERT_NE(rec->archetype, nullptr);
    ASSERT_EQ(rec->archetype->type.size(), 2);
    EXPECT_TRUE(
        (rec->archetype->type[0] == ComponentId::Position && rec->archetype->type[1] == ComponentId::Velocity) ||
        (rec->archetype->type[0] == ComponentId::Velocity && rec->archetype->type[1] == ComponentId::Position)
    );
}

TEST(WorldTest, GetComponent_EntityWithPositionComponent) {
    // Arrange
    World world;
    EntityId e1 = 90;
    world.addComponent(e1, ComponentId::Position);

    // Act
    Position* pos = world.getComponent<Position>(e1);

    // Assert
    ASSERT_NE(pos, nullptr);
    EXPECT_FLOAT_EQ(pos->x, 0.0f);
    EXPECT_FLOAT_EQ(pos->y, 0.0f);
    EXPECT_FLOAT_EQ(pos->z, 0.0f);
}

// FIXME : ECS does not work properly for multiple components
TEST(WorldTest, GetComponent_EntityWithMultipleComponents) {
    // Arrange
    World world;
    EntityId e1 = 8;
    world.addComponent(e1, ComponentId::Position);
    world.addComponent(e1, ComponentId::Velocity);

    // Act
    Position* pos = world.getComponent<Position>(e1);
    Velocity* vel = world.getComponent<Velocity>(e1);

    // Assert
    ASSERT_NE(pos, nullptr);
    ASSERT_NE(vel, nullptr);

    EXPECT_FLOAT_EQ(pos->x, 0.0f);
    EXPECT_FLOAT_EQ(pos->y, 0.0f);
    EXPECT_FLOAT_EQ(pos->z, 0.0f);
    EXPECT_FLOAT_EQ(vel->x, 0.0f);
    EXPECT_FLOAT_EQ(vel->y, 0.0f);
    EXPECT_FLOAT_EQ(vel->z, 0.0f);
}

TEST(WorldTest, RemoveComponent_EntityWithPositionComponent) {
    // Arrange
    World world;
    EntityId e1 = 33;
    world.addComponent(e1, ComponentId::Position);

    // Act
    world.removeComponent(e1, ComponentId::Position);

    // Assert
    Position* pos = world.getComponent<Position>(e1);
    ASSERT_EQ(pos, nullptr);

    Record* rec = world.getArchetypeManager()->getRecord(e1);
    ASSERT_NE(rec, nullptr);
    ASSERT_EQ(rec->archetype->type.size(), 0);
}




