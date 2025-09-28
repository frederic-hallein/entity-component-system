#include <gtest/gtest.h>
#include "entityManager.hpp"

using namespace ecs;

TEST(EntityManagerTestSuite, CreateEntity) {
    // Arrange
    EntityManager entityManager;

    // Act
    EntityId id = entityManager.createEntity();

    // Assert
    EXPECT_EQ(entityManager.getAllEntities().size(), 1);
    EXPECT_EQ(entityManager.getAllEntities()[0], 0);
    EXPECT_EQ(id, 0);
}

TEST(EntityManagerTestSuite, CreateEntities) {
    // Arrange
    EntityManager entityManager;

    // Act
    std::vector<EntityId> ids;
    entityManager.createEntities(10, ids);

    // Assert
    EXPECT_EQ(entityManager.getAllEntities().size(), 10);
    for (u32 i = 0; i < 10; ++i) {
        EXPECT_EQ(entityManager.getAllEntities()[i], i);
        EXPECT_EQ(ids[i], i);
    }
}

TEST(EntityManagerTestSuite, DeleteEntity) {
    // Arrange
    EntityManager entityManager;
    std::vector<EntityId> ids;
    entityManager.createEntities(10, ids);
    EntityId toDelete = 5;

    // Act
    entityManager.deleteEntity(toDelete);

    // Assert
    EXPECT_EQ(entityManager.getAllEntities().size(), 9);
    for (const auto& id : entityManager.getAllEntities()) {
        EXPECT_NE(id, toDelete);
    }
}
