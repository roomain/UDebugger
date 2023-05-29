#pragma once
#include "TDebugObject.h"
#include "TDebugDescriptor.h"
#include <string>

class TestDebugClass;

class TestDescriptor : public Debugger::TClassDescriptor<TestDebugClass>
{
public:
    TestDescriptor();
    ~TestDescriptor() = default;
};


class TestDebugClass : public Debugger::TDebugObject<TestDescriptor>
{
private:
    int m_iMember = 1;
    float m_fMember = 3.4f;
    bool m_bMember = false;
    std::string m_sMember = "member";

public:
    TestDebugClass() = default;
    ~TestDebugClass() = default;
    void setInt(const int a_value);
    void setFloat(const float a_value);
    void setBool(const bool a_value);
    void setString(const std::string& a_value);

    int getInt()const;
    float getFloat()const;
    bool getBool()const;
    std::string getString()const;
};
