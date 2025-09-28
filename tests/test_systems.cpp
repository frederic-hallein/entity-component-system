#include <gtest/gtest.h>
#include "world.hpp"
#include "systems.hpp"

using namespace ecs;

TEST(SystemsTest, MovementSystem_UpdatesPosition) {
    // Arrange
    World world;
    MovementSystem movementSystem;

    EntityId e1 = world.getEntityManager()->createEntity();
    world.addComponent(e1, ComponentId::Position);
    world.addComponent(e1, ComponentId::Velocity);

    auto* pos = world.getComponent<Position>(e1);
    auto* vel = world.getComponent<Velocity>(e1);
    ASSERT_NE(pos, nullptr);
    ASSERT_NE(vel, nullptr);

    pos->x = 1.0f; pos->y = 2.0f; pos->z = 3.0f;
    vel->x = 0.5f; vel->y = -1.0f; vel->z = 2.0f;

    // Act
    movementSystem.update(world, 2.0f);

    // Assert
    EXPECT_FLOAT_EQ(pos->x, 1.0f + 0.5f * 2.0f);
    EXPECT_FLOAT_EQ(pos->y, 2.0f + -1.0f * 2.0f);
    EXPECT_FLOAT_EQ(pos->z, 3.0f + 2.0f * 2.0f);
}