#pragma once
#include <memory>
#include <string>
#include "ISerializer.h"
#include "IDeserializer.h"

namespace Debugger
{
	class IDebugObject;

	/*@brief interface for variable debugging*/
	class IDebugVariable
	{
	protected:
		std::string m_variableName;	/*!< variable name*/

	public:
		IDebugVariable() = delete;
		explicit IDebugVariable(const std::string_view& a_name) : m_variableName{ a_name } {}
		virtual ~IDebugVariable() = default;
		virtual bool isRedadOnly()const = 0;
		virtual void edit(IDebugObject* const a_pObj, IInterfacePropertyEditor& a_editor) = 0;
		virtual void serialize(IDebugObject* const a_pObj, ISerializer& a_ISerializer)const = 0;
		virtual void deserialize(IDebugObject* const a_pObj, IDeserializer& a_IDeserializer)const = 0;
	};
}