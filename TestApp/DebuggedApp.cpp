#include "DebuggedApp.h"
#include "DebugLog.h"
#include <qlistwidget.h>
#include <qstring.h>


DebuggedApp::DebuggedApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.pBtnRefresh, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onRefresh);
    QObject::connect(ui.pBtnLogError, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogError);
    QObject::connect(ui.pBtnLogInfo, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogInfo);
    QObject::connect(ui.pBtnLogWarning, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onLogWarning);

    QObject::connect(ui.pBtnAddDebug, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onAddClass);
    QObject::connect(ui.pBtnAddDerived, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onAddDerived);
    QObject::connect(ui.pBtnRemove, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggedApp::onRemove);
    QObject::connect(ui.listWidget, QOverload<QListWidgetItem *, QListWidgetItem *>::of(&QListWidget::currentItemChanged), this, &DebuggedApp::onRefresh);
}

DebuggedApp::~DebuggedApp()
{}


void DebuggedApp::onRemove()
{
    if (auto pItem = ui.listWidget->currentItem())
    {
        int index = pItem->data(Qt::UserRole + 1).toInt();
        if (pItem->data(Qt::UserRole).toBool())
        {
            m_vDebugDerivedClass.erase(m_vDebugDerivedClass.begin() + index);
        }
        else
        {
            m_vDebugClass.erase(m_vDebugClass.begin() + index);
        }
        ui.listWidget->removeItemWidget(pItem);
    }
    ui.pBtnRemove->setEnabled(ui.listWidget->count() > 0);
}

void DebuggedApp::onAddClass()
{
    auto pDebug = std::make_shared<TestDebugClass>();
    m_vDebugClass.emplace_back(pDebug);
    pDebug->setObjectName(std::string("Debug ") + std::to_string(static_cast<int>(m_vDebugClass.size())));
    QListWidgetItem* pItem = new QListWidgetItem(QString::fromStdString(std::string(pDebug->objectName())));
    pItem->setData(Qt::UserRole, false); 
    pItem->setData(Qt::UserRole + 1, static_cast<int>(m_vDebugClass.size() - 1));
    ui.listWidget->addItem(pItem);
    ui.pBtnRemove->setEnabled(true);
}

void DebuggedApp::onAddDerived()
{
    auto pDebug = std::make_shared<TestDebugDerivedClass>();
    m_vDebugDerivedClass.emplace_back(pDebug);
    pDebug->setObjectName(std::string("Debug Derived ") + std::to_string(static_cast<int>(m_vDebugDerivedClass.size())));
    if (m_vDebugClass.size() > 0)
    {
        int parentIndex = std::rand() % m_vDebugClass.size();
        pDebug->setOwner(m_vDebugClass[parentIndex].get());
    }
    QListWidgetItem* pItem = new QListWidgetItem(QString::fromStdString(std::string(pDebug->objectName())));
    pItem->setData(Qt::UserRole, true);
    pItem->setData(Qt::UserRole + 1, static_cast<int>(m_vDebugDerivedClass.size() - 1));
    ui.listWidget->addItem(pItem);
    ui.pBtnRemove->setEnabled(true);
}

void DebuggedApp::onRefresh()
{
    ui.twObject->clear();
    if (auto pItem = ui.listWidget->currentItem())
    {
        int index = pItem->data(Qt::UserRole + 1).toInt();
        if (pItem->data(Qt::UserRole).toBool())
        {
            auto pDebugged = m_vDebugDerivedClass[index];
            ui.twObject->setColumnCount(2);
            ui.twObject->setRowCount(5);
            ui.twObject->setItem(0, 0, new QTableWidgetItem("m_iMember"));
            ui.twObject->setItem(0, 1, new QTableWidgetItem(QString::number(pDebugged->getInt())));

            ui.twObject->setItem(1, 0, new QTableWidgetItem("m_fMember"));
            ui.twObject->setItem(1, 1, new QTableWidgetItem(QString::number(pDebugged->getFloat())));


            ui.twObject->setItem(2, 0, new QTableWidgetItem("m_bMember"));
            ui.twObject->setItem(2, 1, new QTableWidgetItem(pDebugged->getBool() ? "True" : "False"));


            ui.twObject->setItem(3, 0, new QTableWidgetItem("m_sMember"));
            ui.twObject->setItem(3, 1, new QTableWidgetItem(QString::fromStdString(pDebugged->getString())));

            ui.twObject->setItem(4, 0, new QTableWidgetItem("m_shMember"));
            ui.twObject->setItem(4, 1, new QTableWidgetItem(QString::number(pDebugged->getShort())));
        }
        else
        {
            auto pDebugged = m_vDebugClass[index];
            ui.twObject->setColumnCount(2);
            ui.twObject->setRowCount(4);
            ui.twObject->setItem(0, 0, new QTableWidgetItem("m_iMember"));
            ui.twObject->setItem(0, 1, new QTableWidgetItem(QString::number(pDebugged->getInt())));

            ui.twObject->setItem(1, 0, new QTableWidgetItem("m_fMember"));
            ui.twObject->setItem(1, 1, new QTableWidgetItem(QString::number(pDebugged->getFloat())));


            ui.twObject->setItem(2, 0, new QTableWidgetItem("m_bMember"));
            ui.twObject->setItem(2, 1, new QTableWidgetItem(pDebugged->getBool() ? "True" : "False"));


            ui.twObject->setItem(3, 0, new QTableWidgetItem("m_sMember"));
            ui.twObject->setItem(3, 1, new QTableWidgetItem(QString::fromStdString(pDebugged->getString())));
        }
    }
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
