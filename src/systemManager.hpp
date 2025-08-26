#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "timer.hpp"
#include "system.hpp"
#include "componentManager.hpp"

namespace ecs {

    template<u64 MaxEntities>
    class SystemManager {
    public:
        SystemManager(ComponentManager<MaxEntities>* componentManager)
            : mComponentManager(componentManager) {

            mTimer = std::make_unique<Timer>();
        }

        template<typename SystemType>
        void registerSystem() {
            auto system = std::make_unique<SystemType>();
            system->setComponentManager(mComponentManager);
            system->init();
            mSystems.push_back(std::move(system));
            LOG_INFO("Registered system: ", typeid(SystemType).name());
        }

        template<typename SystemType>
        SystemType* getSystem() {
            for (auto& system : mSystems) {
                if (auto* castedSystem = dynamic_cast<SystemType*>(system.get())) {
                    return castedSystem;
                }
            }
            return nullptr;
        }

        ComponentManager<MaxEntities>* getComponentManager() {
            return mComponentManager;
        }

        void updateSystems(f32 deltaTime) {
            for (auto& system : mSystems) {
                mTimer->start();
                system->update(deltaTime);
                mTimer->end();
                LOG_DEBUG("System: ", typeid(*system).name(), " | Update dt: ", mTimer->getDeltaTime() * 1000.0f, "ms");
            }
        }

        void shutdown() {
            for (auto& system : mSystems) {
                system->shutdown();
            }
            mSystems.clear();
        }

    private:
        std::unique_ptr<Timer> mTimer;
        std::vector<std::unique_ptr<System<MaxEntities>>> mSystems;
        ComponentManager<MaxEntities>* mComponentManager;
    };

}

#endif