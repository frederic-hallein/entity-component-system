#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "world.hpp"
#include "componentTypes.hpp"

namespace ecs {
    class System {
    public:
        virtual ~System() = default;
        virtual void update(World& world, f32 deltaTime) = 0;
    };

    class MovementSystem : public System {
    public:
        void update(World& world, f32 deltaTime) override {
            for (const auto& [entityId, record] : world.getArchetypeManager()->getEntityIndex()) {
                Archetype* arch = record.archetype;
                if (!arch) continue;

                auto posIt = std::find(arch->type.begin(), arch->type.end(), ComponentId::Position);
                auto velIt = std::find(arch->type.begin(), arch->type.end(), ComponentId::Velocity);
                if (posIt != arch->type.end() && velIt != arch->type.end()) {
                    auto* posCol = static_cast<Column<Position>*>(arch->columns[std::distance(arch->type.begin(), posIt)]);
                    auto* velCol = static_cast<Column<Velocity>*>(arch->columns[std::distance(arch->type.begin(), velIt)]);
                    usize row = record.row;

                    posCol->elements[row].x += velCol->elements[row].x * deltaTime;
                    posCol->elements[row].y += velCol->elements[row].y * deltaTime;
                    posCol->elements[row].z += velCol->elements[row].z * deltaTime;
                }
            }
        }
    };

}

#endif