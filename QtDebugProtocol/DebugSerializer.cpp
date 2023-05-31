#include "DebugSerializer.h"
#include <qjsonvalue.h>
#include "DebugProtocol.h"

void DebugDBSerializer::beginDatabase()
{
	m_objList = QJsonArray(); // clear
}

void DebugDBSerializer::serializeObject(const std::string_view& a_name, const std::string_view& a_classname, const int64_t& a_objUID, const int64_t& a_owner)
{
	m_objList.append(DebugProtocol::writeObjectInfo(a_name, a_classname, a_objUID, a_owner));
}


void DebugSerializer::beginObject(const std::string_view& a_name, const Debugger::ClassInfo& a_classInfo, const int64_t& a_objUID, const int64_t& a_owner)
{
	m_currentObjName = a_name;
	m_currentObjOwner = a_owner;
	m_currentUID = a_objUID;
	m_currentClassInfo = a_classInfo;
}

void DebugSerializer::clear()
{
	m_containerObject = QJsonObject();// reset
	m_propList = QJsonArray();// reset
	m_currentObjOwner = 0;
	m_currentUID = 0;
	m_currentClassInfo.m_className = "";
	m_currentClassInfo.m_classSize = 0;
	m_currentClassInfo.m_inheritance = "";
}

void DebugSerializer::endObject()
{
	m_containerObject = DebugProtocol::writeObject(m_currentObjName, m_currentClassInfo, m_currentUID, m_currentObjOwner, m_propList);
}

QString DebugSerializer::toString()const
{
	QJsonDocument doc(m_containerObject);
	return doc.toJson();
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<bool>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<short>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<unsigned short>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<int>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<unsigned int>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<float>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<double>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}

Debugger::ISerializer& DebugSerializer::operator << (const Debugger::VariableInfo<std::string>& a_value)
{
	m_propList.append(QJsonValue(DebugProtocol::writeVariable(a_value)));
	return *this;
}