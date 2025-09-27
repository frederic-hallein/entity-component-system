#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "pch.hpp"
#include "componentTypes.hpp"

namespace ecs {
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

    // Abstract base class for all columns
    struct ColumnBase {
        virtual ~ColumnBase() = default;
        virtual void moveElementFrom(ColumnBase* src, usize srcRow) = 0;
        virtual void removeElement(usize row) = 0;
        virtual usize size() const = 0;
    };

    // Templated column implementation
    template<typename T>
    struct Column : ColumnBase {
        std::vector<T> elements;

        void moveElementFrom(ColumnBase* src, usize srcRow) override {
            if (src) {
                auto* typedSrc = static_cast<Column<T>*>(src);
                elements.push_back(typedSrc->elements[srcRow]);
            } else {
                elements.push_back(T{}); // Default-construct if no source
            }
        }

        void removeElement(usize row) override {
            if (row < elements.size()) {
                elements[row] = elements.back();
                elements.pop_back();
            }
        }

        usize size() const override {
            return elements.size();
        }
    };

    // Inline columns for each component type
    inline Column<Position> positionColumn;
    inline Column<Velocity> velocityColumn;
    inline Column<Acceleration> accelerationColumn;
    inline Column<Health> healthColumn;

    // Use the enum for array size and order
    inline std::array<ColumnBase*, static_cast<usize>(ComponentId::Count)> components = {
        &positionColumn,
        &velocityColumn,
        &accelerationColumn,
        &healthColumn
    };
}

#endif
