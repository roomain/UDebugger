#include "ScopedInstrumentation.h"

namespace Debugger
{

	ScopedInstrumentation::ScopedInstrumentation() : m_timePoint{ std::chrono::system_clock::now() }, m_name { "noname" }
	{
		// nothing todo
	}

	ScopedInstrumentation::ScopedInstrumentation(const std::string& a_name) : m_timePoint{ std::chrono::system_clock::now() }, m_name{a_name}
	{
		// nothing todo
	}

	ScopedInstrumentation::~ScopedInstrumentation()
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_timePoint).count();
		// TODO
	}
}
