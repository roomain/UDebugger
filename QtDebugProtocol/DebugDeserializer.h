#pragma once
#include "IDeserializer.h"
#include <QVariant>
#include "debugprotocol_global.h"

class DEBUGPROTOCOL_EXPORT DebugDeserializer : public Debugger::IDeserializer
{
private:
	QVariant m_value;

public:
	DebugDeserializer() = default;
	~DebugDeserializer() = default;
	void setValue(const QVariant& a_var);
	const IDeserializer& operator >> (bool& a_value)const final;
	const IDeserializer& operator >> (short& a_value)const final;
	const IDeserializer& operator >> (unsigned short& a_value)const final;
	const IDeserializer& operator >> (int& a_value)const final;
	const IDeserializer& operator >> (unsigned int& a_value)const final;
	const IDeserializer& operator >> (float& a_value)const final;
	const IDeserializer& operator >> (double& a_value)const final;
	const IDeserializer& operator >> (std::string& a_value)const final;
	const IDeserializer& operator >> (Debugger::IDebugObject*& a_value)const final;
};

