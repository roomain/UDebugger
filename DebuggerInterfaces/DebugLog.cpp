#include "DebugLog.h"


namespace Debugger
{
	Logger& Logger::instance()
	{
		static Logger s_instance;
		return s_instance;
	}

	void Logger::setCallback(LogCallback a_callback)
	{
		m_callback = a_callback;
	}

	Logger& Logger::operator << (const std::chrono::time_point<std::chrono::system_clock>& a_time)
	{
		Logger::instance().m_currentLog.m_time = a_time;
		return Logger::instance();
	}

	Logger& Logger::operator << (const std::source_location& a_location)
	{
		Logger::instance().m_currentLog.m_location = a_location;
		return Logger::instance();
	}

	Logger& Logger::operator << (const LogSeverity a_severity)
	{
		Logger::instance().m_currentLog.m_severity = a_severity;
		return Logger::instance();
	}

	Logger& Logger::operator << (const std::string_view a_message)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::string(a_message);
		return Logger::instance();
	}

	Logger& Logger::operator << (const char* a_message)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::string(a_message);
		return Logger::instance();
	}

	Logger& Logger::operator << (const bool a_value)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::to_string(a_value);
		return Logger::instance();
	}

	Logger& Logger::operator << (const int a_value)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::to_string(a_value);
		return Logger::instance();
	}

	Logger& Logger::operator << (const float a_value)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::to_string(a_value);
		return Logger::instance();
	}

	Logger& Logger::operator << (const double& a_value)
	{
		Logger::instance().m_currentLog.m_logMessage += " " + std::to_string(a_value);
		return Logger::instance();
	}

	void Logger::flush()
	{
		if (Logger::instance().m_callback)
			Logger::instance().m_callback(Logger::instance().m_currentLog);
		Logger::instance().m_currentLog.m_logMessage = "";
	}
}