#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "pch.hpp"

namespace ecs {
    namespace components {
        struct Position {
            f64 x, y;
        };

        struct Health {
            u32 lifes;
        };

    }

    using namespace components;
}

#endif