#pragma once
#include "TestDebugClass.h"
#include "TDebugDerivedObject.h"
#include <string>

class TestDebugDerivedClass;

class TestDerivedDescriptor : public Debugger::TClassDerivedDescriptor<TestDescriptor, TestDebugDerivedClass>
{
public:
    TestDerivedDescriptor();
    ~TestDerivedDescriptor() = default;
};


class TestDebugDerivedClass : public Debugger::TDebugDerivedObject<TestDebugClass, TestDerivedDescriptor>
{
private:
    short m_shMember = 45;
public:
    TestDebugDerivedClass() = default;
    ~TestDebugDerivedClass() = default;
    void setShort(const short a_value);
    short getShort()const;
};
