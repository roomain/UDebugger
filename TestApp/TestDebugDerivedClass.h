#pragma once
#include "TDebugDescriptor.h"
#include "TestDebugClass.h"
#include "TDebugDerivedObject.h"
#include <string>

class TestDebugDerivedClass;

DECLARE_INHERIT_DESCRIPTOR(TestDebugDerivedClass, TestDebugClass)



class TestDebugDerivedClass : public DEBUG_OBJECT_DERIVED_TEMPLATE(TestDebugDerivedClass, TestDebugClass)
{
private:
    short m_shMember = 45;
public:
    TestDebugDerivedClass() = default;
    ~TestDebugDerivedClass() = default;
    void setShort(const short a_value);
    short getShort()const;
};
