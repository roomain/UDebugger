#pragma once
#include <string>
#include <string_view>
#include "TVarInfo.h"
#include "classInfo.h"

namespace Debugger
{
	class IDebugVariable;

	class IInterfacePropertyEditor
	{
	public:
		virtual void beginObject(const std::string_view& a_name, const ClassInfo& a_classInfo, const int64_t& a_objUID, const int64_t& a_owner) = 0;
		virtual void endObject() = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/bool& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/short& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/unsigned short& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/int& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/unsigned int& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/float& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/double& a_retValue) = 0;
		virtual bool edit(const std::string& a_name, const bool a_readOnly, /*IN/OUT*/std::string& a_retValue) = 0;
	};
}