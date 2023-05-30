# Architecture #
## DebuggerInterfaces ##
![Main architecture](./images/architecture_debugger.png)
- *DebugDatabase* is the container of all class you want to debug
- *IDebugObject* is the base class of all class you want to debug. this class automatically store/remove itself to DebugDatabase 
- *TDebugObject* is a template where parameter is **the descriptor of the class you want to debug**
- *IClassDescriptor* is a interface on Descriptor who describe the class you want to debug
- *TClassDescriptor* is a template where parameter is **the class you want to debug**

Example of debug class *TestDebugClass*:
```C++
#include "TDebugObject.h"
#include "TDebugDescriptor.h"

class TestDebugClass;

// descriptor
class TestDescriptor : public Debugger::TClassDescriptor<TestDebugClass>
{
public:
    TestDescriptor();
    ~TestDescriptor() = default;
};

// class to debug
class TestDebugClass : public Debugger::TDebugObject<TestDescriptor>
{
private:
    // class private members

public:
    TestDebugClass() = default;
    ~TestDebugClass() = default;
    // class public members
};
```

- *TDebugDerivedObject* is a template  where parameters are **the super class and the descriptor of the class you want to debug**
- *TClassDerivedDescriptor* is a template where parameters are **the super class and the class you want to debug**

example:
```C++
#include "TestDebugClass.h"
#include "TDebugDerivedObject.h"

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
    // private members
public:
    TestDebugDerivedClass() = default;
    ~TestDebugDerivedClass() = default;
    // public members
};
```