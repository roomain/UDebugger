#pragma once
#include <string_view>
#include "DebugType.h"

namespace Debugger
{
	template<typename Type>
	struct VariableInfo
	{
		std::string_view m_name;
		Type m_value;
		bool m_bReadOnly;
		EVarType m_type;
	};
}