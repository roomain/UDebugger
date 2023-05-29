#pragma once
#include <memory>
#include <any>
#include <string>
#include "ISerializer.h"
#include "IDeserializer.h"

namespace Debugger
{
	class IDebugObject;

	/*@brief describe class to debug*/
	class IClassDescriptor
	{
	public:
		virtual ~IClassDescriptor() = default;
		virtual std::string className()const noexcept = 0;
		/*@brief serialize described class*/
		virtual void serialize(IDebugObject* const a_pDebug, ISerializer& a_pISerializer)const = 0;
		/*@brief set debug value*/
		virtual void deserialize(const unsigned int a_varIndex, IDebugObject* const a_pDebug, IDeserializer& a_pDeserializer)const = 0;
	};
}