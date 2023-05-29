#include "DebuggerUI.h"
#include "ColorationEditor.h"
#include "DebugWorkspace.h"
#include <qpalette.h>
#include <QTextEdit>
#include <qfiledialog.h>
#include <qdockwidget.h>
#include "DebugExplorer.h"

QPalette paletteTextColor(const QColor& a_color)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, a_color);
    return pal;
}

DebuggerUI::DebuggerUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    delete ui.centralWidget;

    m_pIp = new QLabel("IP address");
    m_pPort = new QLabel("Port");
    m_pConnectInfo = new QLabel("Disconnected");
    m_pConnectInfo->setPalette(paletteTextColor(QColor(255, 0, 0)));
    m_plEdtIP = new QLineEdit();
    m_plEdtIP->setInputMask("000.000.000.000");
    m_plEdtIP->setMaximumWidth(100);
    m_plEdtIP->setText("127.0.0.1");
    m_pspBxPort = new QSpinBox();
    m_pspBxPort->setRange(0, 999999);
    m_pspBxPort->setValue(3001);
    m_pspBxPort->setMaximumWidth(70);
    m_pBtnConnect = new QPushButton("Connect");

    ui.statusBar->addPermanentWidget(m_pIp);
    ui.statusBar->addPermanentWidget(m_plEdtIP);
    ui.statusBar->addPermanentWidget(m_pPort);
    ui.statusBar->addPermanentWidget(m_pspBxPort);
    ui.statusBar->addPermanentWidget(m_pBtnConnect);
    ui.statusBar->addPermanentWidget(m_pConnectInfo);
    
    //---------------------------
    m_pBtnClearLog = new QPushButton("Clear logs");
    m_pBtnClearLog->setVisible(false);
    ui.statusBar->addPermanentWidget(m_pBtnClearLog);

    QObject::connect(&m_client, &DebugClientConnection::sg_connectionError, this, &DebuggerUI::onConnectionError);
    QObject::connect(&m_client, &DebugClientConnection::sg_connected, this, &DebuggerUI::onConnected);
    QObject::connect(&m_client, &DebugClientConnection::sg_disconnected, this, &DebuggerUI::onDisconnected);

    QObject::connect(m_pBtnConnect, QOverload<bool>::of(&QPushButton::clicked), this, &DebuggerUI::onConnect);
    QObject::connect(ui.actionlogs, QOverload<bool>::of(&QAction::triggered), this, &DebuggerUI::onLogDock);
    QObject::connect(ui.actionColoring, QOverload<bool>::of(&QAction::triggered), this, &DebuggerUI::onColoration);
    QObject::connect(ui.actionClone_Explorer, QOverload<bool>::of(&QAction::triggered), this, &DebuggerUI::onCloneExplorer);


    QObject::connect(ui.actionLoad_configuration, QOverload<bool>::of(&QAction::triggered), this, &DebuggerUI::onLoadConf);
    QObject::connect(ui.actionSave_configuration, QOverload<bool>::of(&QAction::triggered), this, &DebuggerUI::onSaveConf);

    m_pExplorer = createExplorer(false);
    addDockWidget(Qt::LeftDockWidgetArea, m_pExplorer);
}

DebuggerUI::~DebuggerUI()
{}

void DebuggerUI::onColoration()
{
    ColorationEditor editor;
    editor.exec();
}


void DebuggerUI::onConnect()
{
    m_client.connectTo(m_plEdtIP->text(), m_pspBxPort->value());
}


void DebuggerUI::onConnected()
{
    m_pConnectInfo->setPalette(paletteTextColor(QColor(0, 255, 0)));
    m_pConnectInfo->setText("Connected");
    const int uid = static_cast<DebugExplorer*>(m_pExplorer->widget())->uid();
    m_client.onAskTree(uid);
}

void DebuggerUI::onDisconnected()
{
    m_pConnectInfo->setPalette(paletteTextColor(QColor(255, 0, 0)));
    m_pConnectInfo->setText("Disconnected");
}

void DebuggerUI::onConnectionError(const QString& a_message)
{
    m_pConnectInfo->setPalette(paletteTextColor(QColor(255, 0, 0)));
    m_pConnectInfo->setText(a_message);
}

void DebuggerUI::onCloneExplorer()
{
    createExplorer(true);
}

QDockWidget* DebuggerUI::createExplorer(const bool a_isClone)
{
    QDockWidget* pDockExplorer = new QDockWidget(this);
    pDockExplorer->setWindowTitle("[Explorer]");
    pDockExplorer->setObjectName("dockExplorer");
    if(a_isClone)
        pDockExplorer->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    else
        pDockExplorer->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    auto pDockWidgetContents = new DebugExplorer();
    pDockWidgetContents->setObjectName("dockWidgetContents");
    pDockExplorer->setWidget(pDockWidgetContents);
    pDockWidgetContents->connect(m_client);
    addDockWidget(Qt::LeftDockWidgetArea, pDockExplorer);
    if (a_isClone && !m_pExplorer->isFloating())
        tabifyDockWidget(pDockExplorer, m_pExplorer);
    
    QObject::connect(pDockWidgetContents, &DebugExplorer::sg_path, pDockExplorer, &QDockWidget::setWindowTitle);
    pDockExplorer->show();
    return pDockExplorer;
}

void DebuggerUI::onLogDock()
{
    m_pBtnClearLog->setVisible(true);
    if (m_pLogDock == nullptr)
    {
        m_pLogDock = new QDockWidget(this);
        m_pLogDock->setObjectName("LOGS");
        m_pLogDock->setWindowTitle("LOGS");
        m_pLogDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        QTextEdit* pLogEdit = new QTextEdit;
        pLogEdit->setReadOnly(true);
        m_pLogDock->setWidget(pLogEdit);
        addDockWidget(Qt::BottomDockWidgetArea, m_pLogDock);
        QObject::connect(m_pLogDock, &QDockWidget::visibilityChanged,
            m_pBtnClearLog, &QPushButton::setVisible);
        QObject::connect(&m_client, &DebugClientConnection::sg_htmlLogs, pLogEdit, &QTextEdit::insertHtml);
    }
    m_pLogDock->show();
}


void DebuggerUI::onLoadConf()
{
    auto filename = QFileDialog::getOpenFileName(this, "Load configuration", "", "XML (*.xml)");
    if (!filename.isEmpty())
    {
        DebugWorkspace::instance().loadConfiguration(filename);
    }
}

void DebuggerUI::onSaveConf()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save configuration", "", "XML (*.xml)");
    if (!filename.isEmpty())
    {
        DebugWorkspace::instance().saveConfiguration(filename);
    }
}