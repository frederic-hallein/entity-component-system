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

    // Explicit enum for component IDs
    enum class ComponentId : u32 {
        Position = 0,
        Velocity,
        Acceleration,
        Health,
        Count
    };

    // Traits to map type to ComponentId
    template<typename T>
    struct ComponentIdTrait;

    template<> struct ComponentIdTrait<Position>     { static constexpr ComponentId id = ComponentId::Position; };
    template<> struct ComponentIdTrait<Velocity>     { static constexpr ComponentId id = ComponentId::Velocity; };
    template<> struct ComponentIdTrait<Acceleration> { static constexpr ComponentId id = ComponentId::Acceleration; };
    template<> struct ComponentIdTrait<Health>       { static constexpr ComponentId id = ComponentId::Health; };

    template<typename T>
    struct Column {
        std::vector<T> elements;
    };

    inline Column<Position> positionColumn;
    inline Column<Velocity> velocityColumn;
    inline Column<Acceleration> accelerationColumn;
    inline Column<Health> healthColumn;

    // Use the enum for array size and order
    inline std::array<void*, static_cast<usize>(ComponentId::Count)> components = {
        &positionColumn,
        &velocityColumn,
        &accelerationColumn,
        &healthColumn
    };
}

#endif