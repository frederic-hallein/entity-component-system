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

TEST(WorldTest, AddComponent_AddHealthToPositionVelocity) {
    // Arrange
    World world;
    EntityId e1 = 1, e2 = 2, e3 = 3, e4 = 4;

    // Add {Position, Velocity} to e1, e2, e3 with random values
    world.addComponent(e1, ComponentId::Position);
    world.addComponent(e1, ComponentId::Velocity);
    world.addComponent(e2, ComponentId::Position);
    world.addComponent(e2, ComponentId::Velocity);
    world.addComponent(e3, ComponentId::Position);
    world.addComponent(e3, ComponentId::Velocity);

    // Set random values for e1, e2, e3
    *world.getComponent<Position>(e1) = Position{1.1f, 2.2f, 3.3f};
    *world.getComponent<Velocity>(e1) = Velocity{4.4f, 5.5f, 6.6f};
    *world.getComponent<Position>(e2) = Position{7.7f, 8.8f, 9.9f};
    *world.getComponent<Velocity>(e2) = Velocity{10.1f, 11.2f, 12.3f};
    *world.getComponent<Position>(e3) = Position{13.4f, 14.5f, 15.6f};
    *world.getComponent<Velocity>(e3) = Velocity{16.7f, 17.8f, 18.9f};

    // Add {Position, Velocity, Health} to e4
    world.addComponent(e4, ComponentId::Position);
    world.addComponent(e4, ComponentId::Velocity);
    world.addComponent(e4, ComponentId::Health);

    *world.getComponent<Position>(e4) = Position{21.1f, 22.2f, 23.3f};
    *world.getComponent<Velocity>(e4) = Velocity{24.4f, 25.5f, 26.6f};
    *world.getComponent<Health>(e4)   = Health{99};

    // Find the archetypes
    Record* rec1 = world.getArchetypeManager()->getRecord(e1);
    Archetype* pvArchetype = rec1->archetype;
    Record* rec4 = world.getArchetypeManager()->getRecord(e4);
    Archetype* pvhArchetype = rec4->archetype;

    // Act
    world.addComponent(e1, ComponentId::Health);

    // Assert
    // e1 should now be in pvhArchetype, as the last row (after e4)
    Record* rec1_after = world.getArchetypeManager()->getRecord(e1);
    ASSERT_EQ(rec1_after->archetype, pvhArchetype);
    usize e1_row = rec1_after->row;
    usize e4_row = rec4->row;
    EXPECT_EQ(e1_row, pvhArchetype->columns[0]->size() - 1);
    EXPECT_EQ(e1_row, e4_row + 1);

    // e3 should now be at the spot where e1 was in pvArchetype (swap-and-pop)
    Record* rec3 = world.getArchetypeManager()->getRecord(e3);
    ASSERT_EQ(rec3->archetype, pvArchetype);
    EXPECT_EQ(rec3->row, rec1->row);

    // pvArchetype should now have 2 entities
    EXPECT_EQ(pvArchetype->columns[0]->size(), 2);

    // pvhArchetype should now have 2 entities
    EXPECT_EQ(pvhArchetype->columns[0]->size(), 2);

    // Check that the data for e1 in pvhArchetype matches what was in pvArchetype
    auto* posCol    = static_cast<Column<Position>*>(pvhArchetype->columns[0]);
    auto* velCol    = static_cast<Column<Velocity>*>(pvhArchetype->columns[1]);
    auto* healthCol = static_cast<Column<Health>*>(pvhArchetype->columns[2]);
    EXPECT_FLOAT_EQ(posCol->elements[e1_row].x, 1.1f);
    EXPECT_FLOAT_EQ(velCol->elements[e1_row].x, 4.4f);
    // Health should be default-initialized (e.g., 0)
    EXPECT_EQ(healthCol->elements[e1_row].lifePoints, 0);
}


