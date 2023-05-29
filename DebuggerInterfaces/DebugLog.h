#pragma once
#include <source_location>
#include <string_view>
#include <chrono>
#include <functional>
#include "debugger_global.h"

namespace Debugger
{
	enum class LogSeverity
	{
		log_info,
		log_warning,
		log_exception,
		log_error,
		log_critical
	};

	struct Log
	{
		LogSeverity m_severity;
		std::chrono::time_point<std::chrono::system_clock> m_time = std::chrono::system_clock::now();
		std::source_location m_location;
		std::string m_logMessage;
	};

	using LogCallback = std::function<void(const Log&)>;

	/*@brief log serializer*/
	class DEBUGGER_INTERFACE_EXPORT Logger
	{
	private:
		Log m_currentLog;
		LogCallback m_callback = nullptr;
		Logger() = default;

	public:
		~Logger() = default;
		static Logger& instance();
		void setCallback(LogCallback a_callback);
		Logger& operator << (const std::chrono::time_point<std::chrono::system_clock>& a_time);
		Logger& operator << (const std::source_location& a_location);
		Logger& operator << (const LogSeverity a_severity);
		Logger& operator << (const std::string_view a_message);
		Logger& operator << (const char* a_message);
		Logger& operator << (const bool a_value);
		Logger& operator << (const int a_value);
		Logger& operator << (const float a_value);
		Logger& operator << (const double& a_value);
		void flush();
	};

}

#define INFO() \
	Debugger::Logger::instance() <<  Debugger::LogSeverity::log_info << std::source_location::current() <<  std::chrono::system_clock::now()

#define WARNING() \
	Debugger::Logger::instance() <<  Debugger::LogSeverity::log_warning << std::source_location::current() <<  std::chrono::system_clock::now()

#define EXCEPTION() \
	Debugger::Logger::instance() <<  Debugger::LogSeverity::log_exception << std::source_location::current() <<  std::chrono::system_clock::now()

#define ERROR() \
	Debugger::Logger::instance() << Debugger::LogSeverity::log_error << std::source_location::current() <<  std::chrono::system_clock::now()

#define CRITICAL() \
	Debugger::Logger::instance() <<  Debugger::LogSeverity::log_critical << std::source_location::current() <<  std::chrono::system_clock::now()