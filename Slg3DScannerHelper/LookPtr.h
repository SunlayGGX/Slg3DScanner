#pragma once


namespace Slg3DScanner
{
    template<class Object>
    class LookPtr
    {
    public:
        using ObjectType = Object;
        using ObjectConstRef = const Object&;
        using ObjectRef = Object&;
        using ObjectPtr = Object*;
        using ObjectConstPtr = const Object*;


    private:
        ObjectPtr m_ptr;


    public:
        LookPtr(ObjectPtr ptr) :
            m_ptr{ ptr }
        {}

        template<class ObjectParent>
        LookPtr(ObjectParent* ptr) :
            m_ptr{ ptr }
        {}

        ~LookPtr() = default;


    public:
        ObjectPtr operator->()
        {
            return m_ptr;
        }

        bool operator==(LookPtr<Object> other) const
        {
            return m_ptr == other.m_ptr;
        }

        operator bool() const
        {
            return m_ptr != nullptr;
        }
    };
}
