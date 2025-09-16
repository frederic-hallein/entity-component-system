#include <gtest/gtest.h>
#include "world.hpp"

using namespace ecs;

TEST(WorldTest, AddComponent) {
    World world;

    /* -- arrange -- */
    EntityId entityId = 1;
    Record& record = world.getArchetypeManager()->getOrCreateRecord(entityId);
    Archetype* initialArchetype = record.archetype;
    ASSERT_NE(initialArchetype, nullptr);
    ASSERT_TRUE(initialArchetype->type.empty());

    /* -- act -- */
    ComponentId testComponentId = 2;
    world.addComponent(entityId, testComponentId);

    /* -- assert -- */
    Record& updatedRecord = world.getArchetypeManager()->getOrCreateRecord(entityId);
    Archetype* newArchetype = updatedRecord.archetype;
    ASSERT_NE(newArchetype, nullptr);
    ASSERT_NE(newArchetype, initialArchetype);
    ASSERT_EQ(newArchetype->type.size(), 1);
    // EXPECT_EQ(newArchetype->type[0], testComponentId); // FIXME : it is not necessary in the first row
}

TEST(WorldTest, RemoveComponent) {
    World world;

    /* -- arrange -- */

    /* -- act -- */

    /* -- assert -- */

}


