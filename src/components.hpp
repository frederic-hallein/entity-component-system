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

    inline Column<Position> positionColumn;
    inline Column<Velocity> velocityColumn;
    inline Column<Acceleration> accelerationColumn;
    inline Column<Health> healthColumn;

    inline std::vector<std::any> components = {
        positionColumn,
        velocityColumn,
        accelerationColumn,
        healthColumn
    };

    // // TODO : use this for addComponent function
    // inline void initComponentValues() {
    //     auto& posCol = std::any_cast<Column<Position>&>(components[0]);
    //     posCol.elements.push_back(Position{1.0f, 2.0f, 3.0f});

    //     auto& velCol = std::any_cast<Column<Velocity>&>(components[1]);
    //     velCol.elements.push_back(Velocity{0.1f, 0.2f, 0.3f});
    // }
}

#endif