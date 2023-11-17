#pragma once
#include <string>
#include <chrono>

namespace Debugger
{
	/*@brief instrumentation data*/
	class ScopedInstrumentation
	{
	private:
		std::chrono::time_point<std::chrono::system_clock> m_timePoint;
		std::string m_name;

	public:
		ScopedInstrumentation();
		explicit ScopedInstrumentation(const std::string& a_name);
		virtual ~ScopedInstrumentation();
	};
}
