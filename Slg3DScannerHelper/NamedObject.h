#pragma once


namespace Slg3DScanner
{
    struct NamedObject
    {
    public:
        std::string m_name;


    public:
        NamedObject(const std::string& name) :
            m_name{ name }
        {}

        NamedObject(const std::string&& name) :
            m_name{ std::move(name) }
        {}
        
        NamedObject(const NamedObject& other) :
            m_name{ other.m_name }
        {}

        NamedObject(NamedObject&& other) :
            m_name{ std::move(other.m_name) }
        {}

        ~NamedObject() = default;


    public:
        NamedObject& operator=(const std::string& name)
        {
            m_name = name;
            return *this;
        }

        NamedObject& operator=(const std::string&& name)
        {
            m_name = std::move(name);
            return *this;
        }

        NamedObject& operator=(const NamedObject& other)
        {
            m_name = other.m_name;
            return *this;
        }

        NamedObject& operator=(NamedObject&& other)
        {
            m_name = std::move(other.m_name);
            return *this;
        }

        bool operator==(const std::string& name) const
        {
            return m_name == name;
        }


        bool operator==(const NamedObject& other) const
        {
            return m_name == other.m_name;
        }

        bool operator!=(const std::string& name) const
        {
            return !(*this == name);
        }

        bool operator!=(const NamedObject& other) const
        {
            return !(*this == other);
        }
    };
}
