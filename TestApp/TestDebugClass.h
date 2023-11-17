#pragma once
#include "TDebugObject.h"
#include "TDebugDescriptor.h"
#include <string>

class TestDebugClass;


DECLARE_DESCRIPTOR(TestDebugClass)

// class to debug
class TestDebugClass : public DEBUG_OBJECT_TEMPLATE(TestDebugClass)
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

