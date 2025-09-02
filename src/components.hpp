#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "pch.hpp"
namespace ecs {
    struct Position {
        f32 x, y, z;
    };

    struct Velocity {
        f32 x, y, z;
    };

    struct Acceleration {
        f32 x, y, z;
    };

    struct Health {
        u8 lifePoints;
    };

    template<typename T>
    struct Column {
        std::vector<T> elements;
    };

    Column<Position> positionColumn;
    Column<Velocity> velocityColumn;
    Column<Acceleration> accelerationColumn;
    Column<Health> healthColumn;

    std::vector<std::any> components = {
        positionColumn,
        velocityColumn,
        accelerationColumn,
        healthColumn
    };

    void printComponentType(u64 id) {
        if (id >= components.size()) {
            LOG_ERROR("Invalid component index\n");
            return;
        }
        const std::any& comp = components[id];
        if (comp.type() == typeid(Column<Position>)) {
            LOG_INFO("Component id = ", id," -> Position component.\n");
        } else if (comp.type() == typeid(Column<Velocity>)) {
            LOG_INFO("Component id = ", id," -> Velocity component.\n");
        } else if (comp.type() == typeid(Column<Acceleration>)) {
            LOG_INFO("Component id = ", id," -> Acceleration component.\n");
        } else if (comp.type() == typeid(Column<Health>)) {
            LOG_INFO("Component id = ", id," -> Health component.\n");
        } else {
            LOG_ERROR("Unknown component type\n");
        }
    }
}

#endif