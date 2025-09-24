#include <gtest/gtest.h>
#include "entityManager.hpp"

using namespace ecs;

// TEST(EntityManagerTestSuite, CreateEntity) {
//     /* -- arrange -- */
//     EntityManager entityManager;

//     /* -- act -- */
//     entityManager.createEntity();

//     /* -- assert -- */
//     EXPECT_EQ(entityManager.getAllEntities().size(), 1);
//     EXPECT_EQ(entityManager.getAllEntities()[0].id, 0);
// }

// TEST(EntityManagerTestSuite, CreateEntities) {
//     /* -- arrange -- */
//     EntityManager entityManager;

//     /* -- act -- */
//     entityManager.createEntities(10);

//     /* -- assert -- */
//     EXPECT_EQ(entityManager.getAllEntities().size(), 10);
//     for (u64 i = 0; i < 10; ++i) {
//         EXPECT_EQ(entityManager.getAllEntities()[i].id, i);
//     }
// }

// TEST(EntityManagerTestSuite, DeleteEntity) {
//     /* -- arrange -- */
//     EntityManager entityManager;
//     entityManager.createEntities(10);
//     Entity toDelete{5};

//     /* -- act -- */
//     entityManager.deleteEntity(toDelete);

//     /* -- assert -- */
//     EXPECT_EQ(entityManager.getAllEntities().size(), 9);
//     for (const auto& entity : entityManager.getAllEntities()) {
//         EXPECT_NE(entity.id, toDelete.id);
//     }
// }