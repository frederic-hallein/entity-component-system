#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "pch.hpp"

namespace ecs {
    namespace components {
        struct Position {
            u64 x, y;
        };

    }

    using namespace components;
}

#endif