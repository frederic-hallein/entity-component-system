#include <gtest/gtest.h>
#include "componentManager.hpp"
#include "components.hpp"

using namespace ecs;

class ComponentManagerTest : public ::testing::Test {
protected:
    ComponentManager<1000> componentManager;
};

TEST_F(ComponentManagerTest, CanAddComponent) {
    Position pos{10, 20};
    u64 entityId = 1;

    componentManager.setComponent(entityId, pos);

    EXPECT_TRUE(componentManager.hasComponent<Position>(entityId));

    auto* retrievedPos = componentManager.getComponent<Position>(entityId);
    ASSERT_NE(retrievedPos, nullptr);
    EXPECT_EQ(retrievedPos->x, 10);
    EXPECT_EQ(retrievedPos->y, 20);
}

TEST_F(ComponentManagerTest, ReturnsNullForNonExistentComponent) {
    u64 entityId = 999;

    auto* pos = componentManager.getComponent<Position>(entityId);
    EXPECT_EQ(pos, nullptr);

    EXPECT_FALSE(componentManager.hasComponent<Position>(entityId));
}