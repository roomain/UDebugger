#pragma once
#include <string>

namespace Debugger
{
	struct ClassInfo
	{
		std::string m_className;
		std::string m_inheritance;
		size_t m_classSize = 0;
	};
}