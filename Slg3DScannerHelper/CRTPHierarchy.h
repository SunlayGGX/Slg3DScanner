#pragma once


namespace Slg3DScanner
{
    /*
    Supposed == defined
    */
    template<class Type>
    struct CRTPNotEqual
    {
    public:
        friend bool operator!=(const Type& right, const Type& left)
        {
            return !(right == left);
        }
    };

    /*
    Supposed == and < defined
    */
    template<class Type>
    struct CRTPFullOrder : virtual protected CRTPNotEqual<Type>
    {
    public:
        friend bool operator>=(const Type& right, const Type& left)
        {
            return !(right < left);
        }

        friend bool operator<=(const Type& right, const Type& left)
        {
            return (right < left) || (right == left);
        }
        
        friend bool operator>(const Type& right, const Type& left)
        {
            return !(right <= left);
        }
    };
}