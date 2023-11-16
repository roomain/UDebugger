#pragma once
#include <string>
#include "DebugType.h"

namespace Debugger
{
	template<typename Type>
	struct VariableInfo
	{
		std::string m_name;
		Type m_value;
		bool m_bReadOnly;
		EVarType m_type;
	};
}