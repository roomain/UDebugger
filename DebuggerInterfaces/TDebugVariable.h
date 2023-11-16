#pragma once
#include <functional>
#include "IDebugVariable.h"
#include "TVarInfo.h"

namespace Debugger
{

    template<typename Type>
    inline EVarType debug_type_of() { return EVarType::type_unknown; }

    template<>
    inline EVarType debug_type_of<bool>() { return EVarType::type_bool; }

    template<>
    inline EVarType debug_type_of<short>() { return EVarType::type_short; }

    template<>
    inline EVarType debug_type_of<unsigned short>() { return EVarType::type_ushort; }

    template<>
    inline EVarType debug_type_of<int>() { return EVarType::type_int; }

    template<>
    inline EVarType debug_type_of<unsigned int>() { return EVarType::type_uint; }

    template<>
    inline EVarType debug_type_of<float>() { return EVarType::type_float; }

    template<>
    inline EVarType debug_type_of<double>() { return EVarType::type_double; }

    template<>
    inline EVarType debug_type_of<std::string>() { return EVarType::type_string; }

    /*@brief template used as base class for debugging variables*/
	template<typename TDebugObject, typename VariableType>
	class TDebugVariable : public IDebugVariable
    {
    public:
        using Setter = std::function<void(TDebugObject* const, const VariableType)>;
        using Getter = std::function<VariableType(TDebugObject* const)>;

    private:
        Setter m_setter;
        Getter m_getter;

    public:
        TDebugVariable(const std::string_view& a_name, Setter a_setter, Getter a_getter) :
            IDebugVariable{ a_name }, m_setter{ a_setter }, m_getter{ a_getter } {}

        ~TDebugVariable()override = default;

        bool isRedadOnly()const final
        {
            return !m_setter;
        }

        void edit(IDebugObject* const a_pObj, IInterfacePropertyEditor& a_editor)
        {
            if (m_getter)
            {
                if (m_setter)
                {
                    m_setter(static_cast<TDebugObject* const>(a_pObj),
                        a_editor.edit(m_variableName, false, m_getter(static_cast<TDebugObject* const>(a_pObj))));
                }
                else
                {
                    a_editor.edit(m_variableName, true, m_getter(static_cast<TDebugObject* const>(a_pObj)));
                }
            }
        }

        inline void serialize(IDebugObject* const a_pObj, ISerializer& a_ISerializer)const final
        {
           if (m_getter && a_pObj)
           {
               VariableType var = m_getter(static_cast<TDebugObject* const>(a_pObj));
               a_ISerializer << VariableInfo{ m_variableName, var, isRedadOnly(), debug_type_of<VariableType>()};
           }               
        }

        inline void deserialize(IDebugObject* const a_pObj, IDeserializer& a_IDeserializer)const final
        {
            if (m_setter && a_pObj)
            {
                VariableType var;
                a_IDeserializer >> var;
                m_setter(static_cast<TDebugObject*const>(a_pObj), var);
            }                
        }

    };
}