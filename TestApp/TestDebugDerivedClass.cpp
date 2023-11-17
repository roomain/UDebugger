#include "TestDebugDerivedClass.h"
#include "TDebugVariable.h"
#include <functional>

BEGIN_IMPLEMENT_DESCRIPTOR(TestDebugDerivedClass)
ADD_MEMBER(TestDebugDerivedClass, short, m_shMember, getShort, setShort)
END_IMPLEMENT_DECRIPTOR

void TestDebugDerivedClass::setShort(const short a_value)
{
	m_shMember = a_value;
}

short TestDebugDerivedClass::getShort()const
{
	return m_shMember;
}