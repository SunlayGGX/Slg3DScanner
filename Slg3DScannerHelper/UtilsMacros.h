#pragma once


#define CONCAT(x,y) x##y
#define CONCATS(x, y, z) CONCAT(x, y)##z

#define STRINGIFY(x) #x

/*
Macro for declaring a get default method. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
*/
#define DECLARE_SIMPLE_GET_ACCESSOR(variableType, variableFuncName) \
variableType get##variableFuncName() const \
{ \
    return m_##variableFuncName; \
} \

/*
Macro for declaring a get default method. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
The return value is a const reference
*/
#define DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(variableType, variableFuncName) \
const variableType& get##variableFuncName() const \
{ \
    return m_##variableFuncName; \
} \

/*
Macro for declaring a set default method. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
*/
#define DECLARE_SIMPLE_SET_ACCESSOR(variableType, variableFuncName) \
void set##variableFuncName(variableType newValue) \
{ \
    m_##variableFuncName = newValue; \
} \

/*
Macro for declaring a set default method. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
The passed value is a const reference
*/
#define DECLARE_SIMPLE_SET_REFERENCE_ACCESSOR(variableType, variableFuncName) \
void set##variableFuncName(const variableType& newValue) \
{ \
    m_##variableFuncName = newValue; \
} \

/*
Macro for declaring a get and set default methods. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
*/
#define DECLARE_SIMPLE_GET_SET_ACCESSOR(variableType, variableFuncName) \
DECLARE_SIMPLE_GET_ACCESSOR(variableType, variableFuncName) \
DECLARE_SIMPLE_SET_ACCESSOR(variableType, variableFuncName)

/*
Macro for declaring a get and set default methods. Pass the type of the variable and its name without its prefix
For example, if the variable is float m_blabla, use the macro with : DECLARE_SIMPLE_GET_ACCESSOR(float, blabla)
the macros used are the references ones
*/
#define DECLARE_SIMPLE_GET_SET_REFERENCE_ACCESSOR(variableType, variableFuncName) \
DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(variableType, variableFuncName) \
DECLARE_SIMPLE_SET_REFERENCE_ACCESSOR(variableType, variableFuncName)


#define DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(Type) template<class Type> class SlgSingletonAllocatorHelper;

/*
Macro for generating :
- prototype
- usings for weak, shared and unique pointer
*/
#define GENERATE_GLOBAL_DATA_FOR(ClassOrStruct, ClassName) \
ClassOrStruct ClassName; \
using ClassName##Ref = std::shared_ptr<ClassName>; \
using ClassName##UniqueRef = std::unique_ptr<ClassName>; \
using ClassName##WeakRef = std::weak_ptr<ClassName>; \
using ClassName##ShArray = std::vector<ClassName##Ref>; \
using ClassName##UArray = std::vector<ClassName##UniqueRef>; \
using ClassName##WArray = std::vector<ClassName##WeakRef>; \


/*
Macro for generating : 
- class prototype
- usings for weak, shared and unique pointer
*/
#define GENERATE_GLOBAL_DATA_FOR_CLASS(ClassName) GENERATE_GLOBAL_DATA_FOR(class, ClassName)

/*
Macro for generating :
- struct prototype
- usings for weak, shared and unique pointer
*/
#define GENERATE_GLOBAL_DATA_FOR_STRUCT(StructName) GENERATE_GLOBAL_DATA_FOR(struct, ClassName)



#define SLG_ERROR_CODE_LOCATION_INFO_WITH_LINE(line)                    "FILE : " __FILE__ " , LINE : " STRINGIFY(line)
#define SLG_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(line)  "FUNCTION : " __FUNCTION__ " , " SLG_ERROR_CODE_LOCATION_INFO_WITH_LINE(line) 


#define SLG_ERROR_CODE_LOCATION_INFO                        SLG_ERROR_CODE_LOCATION_INFO_WITH_LINE(__LINE__)
#define SLG_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO     SLG_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(__LINE__)

#define SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION(Message)   "STATIC ASSERT ERROR :  " SLG_ERROR_CODE_LOCATION_INFO " , ERROR INFO : " Message
#define SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_INSIDE_FUNCTION(Message)    "STATIC ASSERT ERROR :  " SLG_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO " , ERROR INFO : " Message
