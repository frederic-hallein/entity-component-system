#include <gtest/gtest.h>
#include "archetypeManager.hpp"

using namespace ecs;

TEST(ArchetypeManagerTest, CreateArchetype_FirstTimeArchetypeCreation) {
    // Arrange
    ArchetypeManager manager;

    // Act
    Archetype* archetype = manager.createArchetype();

    // Assert
    ASSERT_NE(archetype, nullptr);
    EXPECT_EQ(archetype->id, 1);
    EXPECT_TRUE(archetype->type.empty());
    EXPECT_TRUE(archetype->columns.empty());
}

TEST(ArchetypeManagerTest, GetRecord_RecordNotFound) {
    // Arrange
    ArchetypeManager manager;
    EntityId entityId = 123;

    // Act
    Record* record = manager.getRecord(entityId);

    // Assert
    EXPECT_EQ(record, nullptr);
}

TEST(ArchetypeManagerTest, CreateRecord_FirstTimeRecordCreation) {
    // Arrange
    ArchetypeManager manager;
    EntityId entityId = 42;

    // Act
    Record& record = manager.createRecord(entityId);

    // Assert
    EXPECT_NE(record.archetype, nullptr);
    EXPECT_EQ(record.row, 0);
    EXPECT_EQ(manager.getEntityIndex().size(), 1);
    EXPECT_EQ(manager.getEntityIndex().at(entityId).archetype, record.archetype);
}

TEST(ArchetypeManagerTest, GetRecord_RecordFound) {
    // Arrange
    ArchetypeManager manager;
    EntityId entityId = 123;
    Record& createdRecord = manager.createRecord(entityId);

    // Act
    Record* foundRecord = manager.getRecord(entityId);

    // Assert
    EXPECT_NE(foundRecord, nullptr);
    EXPECT_EQ(foundRecord, &createdRecord);
    EXPECT_EQ(foundRecord->archetype, createdRecord.archetype);
    EXPECT_EQ(foundRecord->row, createdRecord.row);
}

TEST(ArchetypeManagerTest, CreateRecord_TwoSameEntityRecordCreation) {
    // Arrange
    ArchetypeManager manager;
    EntityId entityId = 13;

    // Act
    Record& firstRecord  = manager.createRecord(entityId);
    Record& secondRecord = manager.createRecord(entityId);

    // Assert
    EXPECT_EQ(&firstRecord, &secondRecord);
    EXPECT_EQ(manager.getEntityIndex().size(), 1);
    EXPECT_EQ(firstRecord.row, 0);
    EXPECT_EQ(secondRecord.row, 0);
}

TEST(ArchetypeManagerTest, CreateRecord_TwoDifferentEntityRecordCreation) {
    // Arrange
    ArchetypeManager manager;
    EntityId entityId1 = 100;
    EntityId entityId2 = 200;

    // Act
    Record& record1 = manager.createRecord(entityId1);
    Record& record2 = manager.createRecord(entityId2);

    // Assert
    EXPECT_NE(&record1, &record2);
    EXPECT_EQ(manager.getEntityIndex().size(), 2);
    EXPECT_EQ(record1.row, 0);
    EXPECT_EQ(record2.row, 1);
    EXPECT_NE(record1.archetype, nullptr);
    EXPECT_EQ(record1.archetype, record2.archetype);
}

TEST(ArchetypeManagerTest, GetArchetypeMap_ArchetypeMapNotFound) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(3);

    // Act
    ArchetypeMap* archetypeMap = manager.getArchetypeMap(componentId);

    // Assert
    EXPECT_EQ(archetypeMap, nullptr);
}

TEST(ArchetypeManagerTest, CreateArchetypeMap_FirstTimeArchetypeMapCreation) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(42);

    // Act
    ArchetypeMap& archetypeMap = manager.createArchetypeMap(componentId);

    // Assert
    EXPECT_TRUE(archetypeMap.empty());
    EXPECT_TRUE(manager.getComponentIndex().contains(componentId));
}

TEST(ArchetypeManagerTest, GetArchetypeMap_ArchetypeMapFound) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(77);
    ArchetypeMap& createdMap = manager.createArchetypeMap(componentId);

    // Act
    ArchetypeMap* foundMap = manager.getArchetypeMap(componentId);

    // Assert
    EXPECT_EQ(foundMap, &createdMap);
}

TEST(ArchetypeManagerTest, GetArchetypeRecord_ArchetypeRecordNotFound) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(42);
    ArchetypeId archetypeId = 90;
    ArchetypeMap& archetypeMap = manager.createArchetypeMap(componentId);

    // Act
    ArchetypeRecord* archetypeRecord = manager.getArchetypeRecord(archetypeMap, archetypeId);

    // Assert
    EXPECT_EQ(archetypeRecord, nullptr);
}

TEST(ArchetypeManagerTest, CreateArchetypeRecord_FirstTimeArchetypeRecordCreation) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(12);
    ArchetypeId archetypeId = 43;
    ArchetypeMap& archetypeMap = manager.createArchetypeMap(componentId);

    // Act
    ArchetypeRecord& archetypeRecord = manager.createArchetypeRecord(archetypeMap, archetypeId);

    // Assert
    EXPECT_EQ(&archetypeRecord, &archetypeMap.at(archetypeId));
    EXPECT_EQ(archetypeRecord.column, static_cast<usize>(-1));
}

TEST(ArchetypeManagerTest, GetArchetypeRecord_ArchetypeRecordFound) {
    // Arrange
    ArchetypeManager manager;
    ComponentId componentId = static_cast<ComponentId>(21);
    ArchetypeId archetypeId = 55;
    ArchetypeMap& archetypeMap = manager.createArchetypeMap(componentId);
    ArchetypeRecord& createdArchetypeRecord = manager.createArchetypeRecord(archetypeMap, archetypeId);

    // Act
    ArchetypeRecord* foundArchetypeRecord = manager.getArchetypeRecord(archetypeMap, archetypeId);

    // Assert
    EXPECT_NE(foundArchetypeRecord, nullptr);
    EXPECT_EQ(foundArchetypeRecord, &createdArchetypeRecord);
    EXPECT_EQ(foundArchetypeRecord, &archetypeMap.at(archetypeId));
    EXPECT_EQ(foundArchetypeRecord->column, static_cast<usize>(-1));
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns_EmptyComponentIds) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    std::vector<ComponentId> emptyComponents;

    // Act
    manager.updateArchetypeTypeAndColumns(archetype, emptyComponents);

    // Assert
    EXPECT_TRUE(archetype->type.empty());
    EXPECT_TRUE(archetype->columns.empty());
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns_NonEmptyComponentIds) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    std::vector<ComponentId> components = {static_cast<ComponentId>(2), static_cast<ComponentId>(1)};

    // Act
    manager.updateArchetypeTypeAndColumns(archetype, components);

    // Assert
    EXPECT_EQ(archetype->type.size(), 2);
    EXPECT_EQ(archetype->columns.size(), 2);
    EXPECT_TRUE(std::find(archetype->type.begin(), archetype->type.end(), static_cast<ComponentId>(1)) != archetype->type.end());
    EXPECT_TRUE(std::find(archetype->type.begin(), archetype->type.end(), static_cast<ComponentId>(2)) != archetype->type.end());
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns_SkipExistingComponent) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    archetype->type.push_back(static_cast<ComponentId>(1));
    std::vector<ComponentId> components = {static_cast<ComponentId>(1), static_cast<ComponentId>(2)};

    // Act
    manager.updateArchetypeTypeAndColumns(archetype, components);

    // Assert
    EXPECT_EQ(archetype->type.size(), 2);
    EXPECT_EQ(archetype->columns.size(), 2);
    EXPECT_EQ(std::count(archetype->type.begin(), archetype->type.end(), static_cast<ComponentId>(1)), 1);
    EXPECT_EQ(std::count(archetype->type.begin(), archetype->type.end(), static_cast<ComponentId>(2)), 1);
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns_UnsortedComponentIds) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    std::vector<ComponentId> unsortedComponents = {static_cast<ComponentId>(2), static_cast<ComponentId>(1)};

    // Act
    manager.updateArchetypeTypeAndColumns(archetype, unsortedComponents);

    // Assert
    EXPECT_EQ(archetype->type.size(), 2);
    EXPECT_EQ(archetype->columns.size(), 2);
    EXPECT_EQ(archetype->type[0], static_cast<ComponentId>(1));
    EXPECT_EQ(archetype->type[1], static_cast<ComponentId>(2));
}

TEST(ArchetypeManagerTest, UpdateArchetypeTypeAndColumns_DuplicateComponentIds) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    std::vector<ComponentId> duplicateComponents = {static_cast<ComponentId>(1), static_cast<ComponentId>(1)};

    // Act
    manager.updateArchetypeTypeAndColumns(archetype, duplicateComponents);

    // Assert
    EXPECT_EQ(archetype->type.size(), 1);
    EXPECT_EQ(archetype->columns.size(), 1);
    EXPECT_EQ(archetype->type[0], static_cast<ComponentId>(1));
}

TEST(ArchetypeManagerTest, SetupArchetypeEdges_TypeSizeZero) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    archetype->type.clear();

    // Act
    manager.setupArchetypeEdges(archetype);

    // Assert
    EXPECT_TRUE(archetype->edges.empty());
}

TEST(ArchetypeManagerTest, ComputeTypeHash_UniqueAndConsistent) {
    // Arrange
    ArchetypeManager manager;
    std::vector<ComponentId> type1 = {static_cast<ComponentId>(1), static_cast<ComponentId>(2)};
    std::vector<ComponentId> type2 = {static_cast<ComponentId>(2), static_cast<ComponentId>(1)};
    std::vector<ComponentId> type3 = {static_cast<ComponentId>(1), static_cast<ComponentId>(2)};
    std::vector<ComponentId> emptyType = {};

    // Act
    TypeHash hash1 = manager.computeTypeHash(type1);
    TypeHash hash2 = manager.computeTypeHash(type2);
    TypeHash hash3 = manager.computeTypeHash(type3);
    TypeHash emptyHash = manager.computeTypeHash(emptyType);

    // Assert
    EXPECT_EQ(hash1, hash3);
    EXPECT_NE(hash1, hash2);
    EXPECT_NE(hash1, emptyHash);
    EXPECT_EQ(emptyHash, manager.computeTypeHash());
}

TEST(ArchetypeManagerTest, UpdateArchetypeIndex_AddsArchetypeToIndex) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    archetype->type = {static_cast<ComponentId>(1), static_cast<ComponentId>(2)};
    TypeHash typeHash = manager.computeTypeHash(archetype->type);

    // Act
    manager.updateArchetypeIndex(archetype);

    // Assert
    auto& archetypeIndex = manager.getArchetypeIndex();
    auto it = archetypeIndex.find(typeHash);
    EXPECT_NE(it, archetypeIndex.end());
    EXPECT_EQ(it->second, archetype);
}

TEST(ArchetypeManagerTest, SetupArchetypeEdges_TypeSizeOne) {
    // Arrange
    ArchetypeManager manager;
    Archetype* archetype = manager.createArchetype();
    archetype->type.push_back(static_cast<ComponentId>(5));
    manager.updateArchetypeIndex(archetype);

    // Act
    manager.setupArchetypeEdges(archetype);

    // Assert
    EXPECT_EQ(archetype->edges.size(), 1);
    TypeHash emptyHash = manager.computeTypeHash();
    Archetype* emptyArchetype = manager.getArchetypeIndex().at(emptyHash);

    auto edgeIt = archetype->edges.find(static_cast<ComponentId>(5));
    EXPECT_NE(edgeIt, archetype->edges.end());
    EXPECT_EQ(edgeIt->second.add, nullptr);
    EXPECT_EQ(edgeIt->second.remove, emptyArchetype);

    auto emptyEdgeIt = emptyArchetype->edges.find(static_cast<ComponentId>(5));
    EXPECT_NE(emptyEdgeIt, emptyArchetype->edges.end());
    EXPECT_EQ(emptyEdgeIt->second.add, archetype);
    EXPECT_EQ(emptyEdgeIt->second.remove, nullptr);
}

TEST(ArchetypeManagerTest, SetupArchetypeEdges_SubsetArchetypesPartialExistence) {
    // Arrange
    ArchetypeManager manager;

    Archetype* archetype12 = manager.createArchetype();
    archetype12->type = {static_cast<ComponentId>(1), static_cast<ComponentId>(2)};
    manager.updateArchetypeIndex(archetype12);

    Archetype* archetype23 = manager.createArchetype();
    archetype23->type = {static_cast<ComponentId>(2), static_cast<ComponentId>(3)};
    manager.updateArchetypeIndex(archetype23);

    Archetype* archetype123 = manager.createArchetype();
    archetype123->type = {static_cast<ComponentId>(1), static_cast<ComponentId>(2), static_cast<ComponentId>(3)};
    manager.updateArchetypeIndex(archetype123);

    // Act
    manager.setupArchetypeEdges(archetype123);

    // Assert
    auto edgeIt3 = archetype123->edges.find(static_cast<ComponentId>(3));
    EXPECT_NE(edgeIt3, archetype123->edges.end());
    EXPECT_EQ(edgeIt3->second.remove, archetype12);

    auto edgeIt1 = archetype123->edges.find(static_cast<ComponentId>(1));
    EXPECT_NE(edgeIt1, archetype123->edges.end());
    EXPECT_EQ(edgeIt1->second.remove, archetype23);

    auto edgeIt2 = archetype123->edges.find(static_cast<ComponentId>(2));
    EXPECT_EQ(edgeIt2, archetype123->edges.end());

    auto subsetEdgeIt3 = archetype12->edges.find(static_cast<ComponentId>(3));
    EXPECT_NE(subsetEdgeIt3, archetype12->edges.end());
    EXPECT_EQ(subsetEdgeIt3->second.add, archetype123);

    auto subsetEdgeIt1 = archetype23->edges.find(static_cast<ComponentId>(1));
    EXPECT_NE(subsetEdgeIt1, archetype23->edges.end());
    EXPECT_EQ(subsetEdgeIt1->second.add, archetype123);
}

