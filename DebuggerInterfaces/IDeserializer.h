#pragma once
#include <string>

namespace Debugger
{
	class IDebugObject;

	/*@brief deserializer interface*/
	class IDeserializer
	{
	public:
		virtual ~IDeserializer() = default;
		virtual const IDeserializer& operator >> (bool& a_value)const = 0;
		virtual const IDeserializer& operator >> (short& a_value)const = 0;
		virtual const IDeserializer& operator >> (unsigned short& a_value)const = 0;
		virtual const IDeserializer& operator >> (int& a_value)const = 0;
		virtual const IDeserializer& operator >> (unsigned int& a_value)const = 0;
		virtual const IDeserializer& operator >> (float& a_value)const = 0;
		virtual const IDeserializer& operator >> (double& a_value)const = 0;
		virtual const IDeserializer& operator >> (std::string& a_value)const = 0;
		virtual const IDeserializer& operator >> (IDebugObject*& a_value)const = 0;
	};
}