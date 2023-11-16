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
		virtual bool edit(const bool a_value) = 0;
		virtual short edit(const short a_value) = 0;
		virtual unsigned short edit(const unsigned short a_value) = 0;
		virtual int edit(const int a_value) = 0;
		virtual unsigned int edit(const unsigned int a_value) = 0;
		virtual float edit(const float a_value) = 0;
		virtual double edit(const double& a_value) = 0;
		virtual std::string edit(const std::string& a_value) = 0;
	};
}