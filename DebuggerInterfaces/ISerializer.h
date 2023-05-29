#pragma once
#include <string_view>
#include <cstdint>
#include "TVarInfo.h"

namespace Debugger
{
	/*@brief interface for serialization*/
	class ISerializer
	{
	public:
		virtual ~ISerializer() = default;
		virtual void beginObject(const std::string_view& a_name, const std::string_view& a_classname, const uint64_t& a_objUID, const uint64_t& a_owner) = 0;
		virtual void endObject() = 0;
		virtual ISerializer& operator << (const VariableInfo<bool>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<short>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<unsigned short>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<int>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<unsigned int>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<float>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<double>& a_value) = 0;
		virtual ISerializer& operator << (const VariableInfo<std::string>& a_value) = 0;
	};

	class IDatabaseSerializer
	{
	public:
		virtual ~IDatabaseSerializer() = default;
		virtual void beginDatabase() = 0;
		virtual void serializeObject(const std::string_view& a_name, const std::string_view& a_classname, const uint64_t& a_objUID, const uint64_t& a_owner) = 0;
		virtual void endDatabase() = 0;
	};
}