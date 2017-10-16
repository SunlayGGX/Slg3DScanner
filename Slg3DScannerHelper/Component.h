#pragma once

namespace Slg3DScanner
{
    class Component : virtual public std::enable_shared_from_this<Component>
    {
    public:
        Component();
        virtual ~Component();

        virtual void registerComponent() = 0;
        virtual void unregisterComponent() = 0;

        virtual void destroy() = 0;

        virtual void update(float deltaTime) = 0;

        virtual bool isRegistered() const;
    };
}


