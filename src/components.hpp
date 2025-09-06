#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "pch.hpp"

// template<typename Component>
// using Column = std::vector<Component>;

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



    // std::vector<std::any> components = {
    //     Column<Position>{},
    //     Column<Velocity>{},
    //     Column<Acceleration>{},
    //     Column<Health>{}
    // };

    // void printComponentType(u32 id) {
    //     if (id >= components.size()) {
    //         LOG_ERROR("Invalid component index\n");
    //         return;
    //     }
    //     const std::any& comp = components[id];
    //     if (comp.type() == typeid(Column<Position>)) {
    //         LOG_INFO("Component id = ", id," -> Position component.\n");
    //     } else if (comp.type() == typeid(Column<Velocity>)) {
    //         LOG_INFO("Component id = ", id," -> Velocity component.\n");
    //     } else if (comp.type() == typeid(Column<Acceleration>)) {
    //         LOG_INFO("Component id = ", id," -> Acceleration component.\n");
    //     } else if (comp.type() == typeid(Column<Health>)) {
    //         LOG_INFO("Component id = ", id," -> Health component.\n");
    //     } else {
    //         LOG_ERROR("Unknown component type\n");
    //     }
    // }

    // // TODO : use this for addComponent function
    // inline void initComponentValues() {
    //     auto& posCol = std::any_cast<Column<Position>&>(components[0]);
    //     posCol.elements.push_back(Position{1.0f, 2.0f, 3.0f});

    //     auto& velCol = std::any_cast<Column<Velocity>&>(components[1]);
    //     velCol.elements.push_back(Velocity{0.1f, 0.2f, 0.3f});
    // }
}

#endif