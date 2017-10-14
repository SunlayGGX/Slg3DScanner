#pragma once

#include "CRTPHierarchy.h"

namespace Slg3DScanner
{
    struct FacetteTraits
    {
    public:
        using IDValue = unsigned short;
    };

    /*
    Do not derive directly from this.
    Use FacetteAttribute.
    */
    template<class AttributeType> 
    class Facette : public FacetteTraits
    {
    protected:
        struct ID
        {
        private:
            static IDValue s_attributor;


        public:
            IDValue m_value;


        public:
            ID() :
                m_value{ s_attributor++ }
            {}

            ~ID() = default;
        };


    };


    template<class Type, class AttributeType>
    class FacetteAttribute : 
        virtual public Facette<AttributeType>,
        virtual protected CRTPFullOrder<FacetteAttribute<Type, AttributeType>>
    {
    protected:
        static const ID ID;


    public:
        FacetteAttribute() = default;
        ~FacetteAttribute() = default;


    public:
        static constexpr FacetteTraits::IDValue getID()
        {
            return ID.m_value;
        }


    public:
        bool operator==(const FacetteAttribute<Type, AttributeType>& other)
        {
            return this->ID.m_value == other.ID.m_value;
        }

        bool operator<(const FacetteAttribute<Type, AttributeType>& other)
        {
            return this->ID.m_value < other.ID.m_value;
        }
    };


    template<class AttributeType> FacetteTraits::IDValue Facette<AttributeType>::ID::s_attributor = 0;
    template<class Type, class AttributeType> const typename Facette<AttributeType>::ID FacetteAttribute<Type, AttributeType>::ID;


#define GENERATE_CODE_FROM_FacetteAttribute(Type, AttributeType) \
    public: \
    using FacetteAttribute<Type, AttributeType>::getID
}


