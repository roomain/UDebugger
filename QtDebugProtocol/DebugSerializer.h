#pragma once
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <ISerializer.h>
#include "debugprotocol_global.h"

class DEBUGPROTOCOL_EXPORT DebugDBSerializer : public Debugger::IDatabaseSerializer
{
private:
	QJsonArray m_objList;

public:
	~DebugDBSerializer() = default;
	[[nodiscard]]const QJsonArray& list()const { return m_objList; }
	void beginDatabase()final;
	void serializeObject(const std::string_view& a_name, const std::string_view& a_classname, const uint64_t& a_objUID, const uint64_t& a_owner)final;
	void endDatabase()final {}
};


class DEBUGPROTOCOL_EXPORT DebugSerializer : public Debugger::ISerializer
{
private:
	QJsonObject m_containerObject;
	QJsonArray m_propList;

	std::string_view m_currentObjName;
	std::string_view m_currentClassName;
	uint64_t m_currentUID = 0;
	uint64_t m_currentObjOwner = 0;


public:
	DebugSerializer() = default;
	~DebugSerializer() = default;
	void clear();
	QString toString()const;
	uint64_t objectId()const { return m_currentUID; }
	[[nodiscard]] inline const QJsonObject& object()const { return  m_containerObject; }
	void beginObject(const std::string_view& a_name, const std::string_view& a_classname, const uint64_t& a_objUID, const uint64_t& a_owner) final;
	void endObject() final;
	ISerializer& operator << (const Debugger::VariableInfo<bool>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<short>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<unsigned short>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<int>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<unsigned int>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<float>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<double>& a_value) final;
	ISerializer& operator << (const Debugger::VariableInfo<std::string>& a_value) final;
};

