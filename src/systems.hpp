#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "system.hpp"
#include "components.hpp"

namespace ecs {

    // Movement System - handles Position + Velocity
    template<u64 MaxEntities>
    class MovementSystem : public System<MaxEntities> {
    public:
        void update(f32 deltaTime) override {
            for (u64 entityId = 0; entityId < MaxEntities; ++entityId) {
                bool hasComponents = this->template entityHasComponents<Position, Velocity>(entityId);
                if (!hasComponents) {
                    continue;
                }

                auto* position = this->mComponentManager->template getComponent<Position>(entityId);
                auto* velocity = this->mComponentManager->template getComponent<Velocity>(entityId);
                if (position && velocity) {
                    position->x += velocity->dx * deltaTime;
                    position->y += velocity->dy * deltaTime;
                    LOG_DEBUG("Entity ", entityId, " moved to (", position->x, ", ", position->y, ")");
                }
            }
        }
    };

    // Health System - handles health regeneration
    template<u64 MaxEntities>
    class HealthSystem : public System<MaxEntities> {
    public:
        void update(f32 deltaTime) override {
            for (u64 entityId = 0; entityId < MaxEntities; ++entityId) {
                bool hasComponents = this->template entityHasComponents<Health>(entityId);
                if (!hasComponents) {
                    continue;
                }

                auto* health = this->mComponentManager->template getComponent<Health>(entityId);
                // if (health && health->current < health->max) {
                //     health->current += health->regeneration * deltaTime;

                //     // Cap at max health
                //     if (health->current > health->max) {
                //         health->current = health->max;
                //     }

                //     LOG_DEBUG("Entity ", entityId, " health: ", health->current, "/", health->max);
                // }
            }
        }
    };

    // Render System - handles rendering entities with Position
    template<u64 MaxEntities>
    class RenderSystem : public System<MaxEntities> {
    public:
        void update(f32 deltaTime) override {
            for (u64 entityId = 0; entityId < MaxEntities; ++entityId) {
                bool hasComponents = this->template entityHasComponents<Position>(entityId);
                if (!hasComponents) {
                    continue;
                }

                auto* position = this->mComponentManager->template getComponent<Position>(entityId);

                if (position) {
                    // call rendering code
                    LOG_DEBUG("Rendering entity ", entityId, " at (", position->x, ", ", position->y, ")");
                }
            }
        }
    };

}

#endif