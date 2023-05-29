#include "TestDebugClass.h"
#include "TDebugVariable.h"
#include <functional>

TestDescriptor::TestDescriptor() : Debugger::TClassDescriptor<TestDebugClass>()
{
	using namespace Debugger;
	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugClass, int>>("m_iMember",
		std::bind_front(&TestDebugClass::setInt), std::bind_front(&TestDebugClass::getInt)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugClass, float>>("m_fMember",
		std::bind_front(&TestDebugClass::setFloat), std::bind_front(&TestDebugClass::getFloat)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugClass, bool>>("m_bMember",
		std::bind_front(&TestDebugClass::setBool), std::bind_front(&TestDebugClass::getBool)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugClass, std::string>>("m_sMember",
		std::bind_front(&TestDebugClass::setString), std::bind_front(&TestDebugClass::getString)));
}



void TestDebugClass::setInt(const int a_value)
{
	m_iMember = a_value;
}

void TestDebugClass::setFloat(const float a_value)
{
	m_fMember = a_value;
}

void TestDebugClass::setBool(const bool a_value)
{
	m_bMember = a_value;
}

void TestDebugClass::setString(const std::string& a_value)
{
	m_sMember = a_value;
}

int TestDebugClass::getInt()const
{
	return m_iMember;
}
float TestDebugClass::getFloat()const
{
	return m_fMember;
}
bool TestDebugClass::getBool()const
{
	return m_bMember;
}
std::string TestDebugClass::getString()const
{
	return m_sMember;
}
