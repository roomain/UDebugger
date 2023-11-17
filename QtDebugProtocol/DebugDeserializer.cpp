#include "DebugDeserializer.h"


void DebugDeserializer::setValue(const QVariant& a_var)
{
	m_value = a_var;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (bool& a_value)const
{
	a_value = m_value.toBool();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (short& a_value)const
{
	a_value = static_cast<short>(m_value.toInt());
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (unsigned short& a_value)const
{
	a_value = static_cast<unsigned short>(m_value.toUInt());
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (int& a_value)const
{
	a_value = m_value.toInt();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (unsigned int& a_value)const
{
	a_value = m_value.toUInt();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (float& a_value)const
{
	a_value = m_value.toFloat();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (double& a_value)const
{
	a_value = m_value.toDouble();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (std::string& a_value)const
{
	a_value = m_value.toString().toStdString();
	return *this;
}

const Debugger::IDeserializer& DebugDeserializer::operator >> (Debugger::IDebugObject* a_value)const
{
	//
	return *this;
}