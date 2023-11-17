#pragma once
#include <string>

namespace Debugger
{
	/*@brief define variable type*/
	enum class EVarType
	{
		type_unknown,
		type_bool,
		type_short,
		type_ushort,
		type_int,
		type_uint,
		type_float,
		type_double,
		type_string,
		type_link
	};

}