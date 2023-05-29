#include "TestDebugDerivedClass.h"
#include "TDebugVariable.h"
#include <functional>

TestDerivedDescriptor::TestDerivedDescriptor() : 
	Debugger::TClassDerivedDescriptor<TestDescriptor, TestDebugDerivedClass>()
{
	using namespace Debugger;
	/*m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugDerivedClass, int>>("m_iMember",
		std::bind_front(&TestDebugClass::setInt), std::bind_front(&TestDebugClass::getInt)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugDerivedClass, float>>("m_fMember",
		std::bind_front(&TestDebugClass::setFloat), std::bind_front(&TestDebugClass::getFloat)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugDerivedClass, bool>>("m_bMember",
		std::bind_front(&TestDebugClass::setBool), std::bind_front(&TestDebugClass::getBool)));

	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugDerivedClass, std::string>>("m_sMember",
		std::bind_front(&TestDebugClass::setString), std::bind_front(&TestDebugClass::getString)));
	*/
	m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<TestDebugDerivedClass, short>>("m_shMember",
		std::bind_front(&TestDebugDerivedClass::setShort), std::bind_front(&TestDebugDerivedClass::getShort)));
}

void TestDebugDerivedClass::setShort(const short a_value)
{
	m_shMember = a_value;
}

short TestDebugDerivedClass::getShort()const
{
	return m_shMember;
}