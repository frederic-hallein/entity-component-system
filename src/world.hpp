#ifndef WORLD_HPP
#define WORLD_HPP

#include "pch.hpp"
// #include "archetypeManager.hpp"
#include "components.hpp"

namespace ecs {
    struct Entity {
        u64 id;
    };

    class World {
    public:
        World() {
            // mArchetypeManager = std::make_unique<ArchetypeManager>();
        }

        void createEntity() {
            Entity e{mEntityId++};
            mEntities.push_back(e);
        };

        void createEntities(usize amount) {
            for (u64 i = 0; i < amount; ++i) {
                createEntity();
            }
        }


        template<typename Component>
        void addComponent(
            Entity entity,
            const Component& component
        )
        {
            // check if entity has archetype with this component
            // If true : do nothing + LOG
            // If false : check if archetype exist with current archetype with new component
            //      If True : change to this new archetype
            //      If False : create new archetype and assign address to entity


        }

        template<typename... Components>
        void addComponents(Entity entity, const Components&... components) {
            (addComponent(entity, components), ...);
        }

        void deleteEntity(const Entity& entity) {
            auto it = std::find_if(mEntities.begin(), mEntities.end(),
                [entity](const Entity& e) { return e.id == entity.id; });

            if (it != mEntities.end()) {
                std::iter_swap(it, mEntities.end() - 1);
                mEntities.pop_back();
            }

            LOG_INFO("Deleted entity ID = ", entity.id);
        }

        const std::vector<Entity>& getAllEntities() const {
            return mEntities;
        }

    private:
        u64 mEntityId = 0;
        std::vector<Entity> mEntities;

        // std::unique_ptr<ArchetypeManager> mArchetypeManager;
    };
}

#endif