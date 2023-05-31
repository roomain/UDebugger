#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DebuggedApp.h"
#include "TestDebugDerivedClass.h"
#include <vector>

class DebuggedApp : public QMainWindow
{
    Q_OBJECT

public:
    DebuggedApp(QWidget *parent = nullptr);
    ~DebuggedApp();

private:
    Ui::DebuggedAppClass ui;
    std::vector<std::shared_ptr<TestDebugClass>> m_vDebugClass;
    std::vector<std::shared_ptr<TestDebugDerivedClass>> m_vDebugDerivedClass;

    void onAddClass();
    void onAddDerived();
    void onRemove();

    void onRefresh();
    void onLogError();
    void onLogInfo();
    void onLogWarning();
};
