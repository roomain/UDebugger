#pragma once
#include <string>
#include <chrono>

namespace Debugger
{
	/*@brief instrumentation data*/
	struct InstrumentationData
	{
		std::chrono::duration<int, std::milli> m_duration;// in ms
		std::string m_name;
	};
}