#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DebuggedApp.h"
#include "TestDebugDerivedClass.h"


class DebuggedApp : public QMainWindow
{
    Q_OBJECT

public:
    DebuggedApp(QWidget *parent = nullptr);
    ~DebuggedApp();

private:
    Ui::DebuggedAppClass ui;
    TestDebugClass m_debuggedParent;
    TestDebugDerivedClass m_debugged;

    void onRefresh();
    void onLogError();
    void onLogInfo();
    void onLogWarning();
};
