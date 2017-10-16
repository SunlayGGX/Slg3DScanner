#pragma once

#include "DirectXUtilitary.h"
#include "Uncopyable.h"
#include "CRTPHierarchy.h"


namespace Slg3DScanner
{
    template<class DXPtr>
    class UniqueDX : 
        private Uncopyable,
        private CRTPNotEqual<UniqueDX<DXPtr>>
    {
    private:
        DXPtr* m_Ptr;


    public:
        UniqueDX() :
            m_Ptr{ nullptr }
        {}

        UniqueDX(DXPtr* ptr) :
            m_Ptr{ ptr }
        {}

        UniqueDX(UniqueDX&& other) :
            m_Ptr{ nullptr }
        {
            std::swap(m_Ptr, other.m_Ptr);
        }

        ~UniqueDX()
        {
            DXRelease(m_Ptr);
        }


    public:
        const DXPtr*const& get() const
        {
            return m_Ptr;
        }

        DXPtr*& get()
        {
            return m_Ptr;
        }

        DXPtr* release()
        {
            DXPtr* intermediary = this->get();
            m_Ptr = nullptr;
            return intermediary;
        }


    public:
        DXPtr* operator->()
        {
            return this->get();
        }

        DXPtr& operator*()
        {
            return *this->get();
        }

        DXPtr*const * operator&()
        {
            return &this->get();
        }

        const DXPtr* operator->() const
        {
            return this->get();
        }

        const DXPtr& operator*() const
        {
            return *this->get();
        }

        const DXPtr*const * operator&() const
        {
            return &this->get();
        }

        UniqueDX&& operator=(UniqueDX&& other)
        {
            std::swap(m_Ptr, other.m_Ptr);
            return *this;
        }

        UniqueDX& operator=(DXPtr* other)
        {
            DXRelease(m_Ptr);
            m_Ptr = other;
            return *this;
        }

        bool operator==(const UniqueDX& other) const
        {
            return m_Ptr == other.m_Ptr;
        }
    };
}
