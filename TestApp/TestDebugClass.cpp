#include "TestDebugClass.h"
#include "TDebugVariable.h"
#include <functional>



BEGIN_IMPLEMENT_DESCRIPTOR(TestDebugClass)
ADD_MEMBER(TestDebugClass, int, m_iMember, getInt, setInt)
ADD_MEMBER(TestDebugClass, float, m_fMember, getFloat, setFloat)
ADD_MEMBER(TestDebugClass, bool, m_bMember, getBool, setBool)
ADD_MEMBER(TestDebugClass, std::string, m_sMember, getString, setString)
END_IMPLEMENT_DECRIPTOR

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
