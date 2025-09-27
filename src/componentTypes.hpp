#ifndef COMPONENT_TYPES_HPP
#define COMPONENT_TYPES_HPP

#include "pch.hpp"

namespace ecs {
    struct Position {
        f32 x, y, z;
        Position() : x(0.0f), y(0.0f), z(0.0f) {}
        Position(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    };

    struct Velocity {
        f32 x, y, z;
        Velocity() : x(0.0f), y(0.0f), z(0.0f) {}
        Velocity(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    };

    struct Acceleration {
        f32 x, y, z;
        Acceleration() : x(0.0f), y(0.0f), z(0.0f) {}
        Acceleration(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    };

    struct Health {
        u8 lifePoints;
        Health() : lifePoints(0) {}
        Health(u8 lp) : lifePoints(lp) {}
    };
}

#endif
