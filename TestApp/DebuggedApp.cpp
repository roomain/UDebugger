#include "DebuggedApp.h"
#include "DebugLog.h"


DebuggedApp::DebuggedApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_debuggedParent.setObjectName("Debug parent");
    m_debugged.setObjectName("Debug object");
    m_debugged.setOwner(&m_debuggedParent);
    onRefresh();
    QObject::connect(ui.pBtnRefresh, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onRefresh);
    QObject::connect(ui.pBtnLogError, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogError);
    QObject::connect(ui.pBtnLogInfo, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogInfo);
    QObject::connect(ui.pBtnLogWarning, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogWarning);
}

DebuggedApp::~DebuggedApp()
{}

void DebuggedApp::onRefresh()
{

    ui.twParent->clear();
    ui.twParent->setColumnCount(2);
    ui.twParent->setRowCount(4);
    ui.twParent->setItem(0, 0, new QTableWidgetItem("m_iMember"));
    ui.twParent->setItem(0, 1, new QTableWidgetItem(QString::number(m_debuggedParent.getInt())));

    ui.twParent->setItem(1, 0, new QTableWidgetItem("m_fMember"));
    ui.twParent->setItem(1, 1, new QTableWidgetItem(QString::number(m_debuggedParent.getFloat())));


    ui.twParent->setItem(2, 0, new QTableWidgetItem("m_bMember"));
    ui.twParent->setItem(2, 1, new QTableWidgetItem(m_debuggedParent.getBool() ? "True" : "False"));


    ui.twParent->setItem(3, 0, new QTableWidgetItem("m_sMember"));
    ui.twParent->setItem(3, 1, new QTableWidgetItem(QString::fromStdString(m_debuggedParent.getString())));

    ui.twChild->clear();
    ui.twChild->setColumnCount(2);
    ui.twChild->setRowCount(5);
    ui.twChild->setItem(0, 0, new QTableWidgetItem("m_iMember"));
    ui.twChild->setItem(0, 1, new QTableWidgetItem(QString::number(m_debugged.getInt())));

    ui.twChild->setItem(1, 0, new QTableWidgetItem("m_fMember"));
    ui.twChild->setItem(1, 1, new QTableWidgetItem(QString::number(m_debugged.getFloat())));

    ui.twChild->setItem(2, 0, new QTableWidgetItem("m_bMember"));
    ui.twChild->setItem(2, 1, new QTableWidgetItem(m_debugged.getBool() ? "True" : "False"));

    ui.twChild->setItem(3, 0, new QTableWidgetItem("m_sMember"));
    ui.twChild->setItem(3, 1, new QTableWidgetItem(QString::fromStdString(m_debugged.getString())));

    ui.twChild->setItem(4, 0, new QTableWidgetItem("m_shMember"));
    ui.twChild->setItem(4, 1, new QTableWidgetItem(QString::number(m_debugged.getShort())));

}


void DebuggedApp::onLogError()
{
    ERROR() << "Log error";
    Debugger::Logger::instance().flush();
}

void DebuggedApp::onLogInfo()
{
    INFO() << "Log info";
    Debugger::Logger::instance().flush();
}

void DebuggedApp::onLogWarning()
{
    WARNING() << "Log warning";
    Debugger::Logger::instance().flush();
}
