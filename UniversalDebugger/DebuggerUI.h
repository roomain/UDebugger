#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DebuggerUI.h"
#include <qlabel.h>
#include <QPushButton>
#include <qlineedit.h>
#include <qspinbox.h>
#include "DebugClientConnection.h"


class DebuggerUI : public QMainWindow
{
    Q_OBJECT

public:
    DebuggerUI(QWidget *parent = nullptr);
    ~DebuggerUI();

private:
    Ui::DebuggerUIClass ui;

    // TCP Connection
    QLabel* m_pIp = nullptr;
    QLabel* m_pPort = nullptr;
    QLabel* m_pConnectInfo = nullptr;
    QLineEdit* m_plEdtIP = nullptr;
    QSpinBox* m_pspBxPort = nullptr;
    QPushButton* m_pBtnConnect = nullptr;
    //---------------------------
    QPushButton* m_pBtnClearLog = nullptr;
    QDockWidget* m_pLogDock = nullptr;
    //---------------------------
    DebugClientConnection m_client;
    QDockWidget* m_pExplorer;

    void onLogDock();
    void onColoration();
    void onConnect();
    void onCloneExplorer();
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString& a_message);
    QDockWidget* createExplorer(const bool a_isClone);

    void onLoadConf();
    void onSaveConf();
};
