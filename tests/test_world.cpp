#include <gtest/gtest.h>
#include "world.hpp"

using namespace ecs;

TEST(WorldTest, CreateEntity) {
    World world;
    world.getEntityManager()->createEntity();
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 1);
    EXPECT_EQ(world.getEntityManager()->getAllEntities()[0].id, 0);
}

TEST(WorldTest, CreateEntities) {
    World world;
    world.getEntityManager()->createEntities(10);
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 10);
    for (u64 i = 0; i < 10; ++i) {
        EXPECT_EQ(world.getEntityManager()->getAllEntities()[i].id, i);
    }
}

TEST(WorldTest, DeleteEntity) {
    World world;
    world.getEntityManager()->createEntities(10);
    Entity toDelete{5};
    world.getEntityManager()->deleteEntity(toDelete);
    EXPECT_EQ(world.getEntityManager()->getAllEntities().size(), 9);
    for (const auto& entity : world.getEntityManager()->getAllEntities()) {
        EXPECT_NE(entity.id, toDelete.id);
    }
}

// // TODO : add more checks
// TEST(WorldTest, AddComponentToEntity) {
//     World world;
//     world.createEntity();
//     Entity entity = world.getAllEntities()[0];
//     Position pos{1.0f, 2.0f, 3.0f};
//     EXPECT_NO_THROW(world.addComponent(entity, pos));
// }