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

    // Add more components here


    template<typename T>
    struct ComponentTypeId {
        static u32 value() {
            static u32 id = counter++;
            return id;
        }
    private:
        static u32 counter;
    };

    template<typename T>
    u32 ComponentTypeId<T>::counter = 0;
}

#endif