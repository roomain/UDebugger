#include "QDebuggerServer.h"

QDebuggerServer::QDebuggerServer() : QTcpServer()
{
	QObject::connect(this, &QTcpServer::newConnection, this, &QDebuggerServer::onNewConnection);
}

void QDebuggerServer::setDatabase(const Debugger::DebugDatabasePtr& a_pDatabase)
{
	m_pDatabase = a_pDatabase;
}

void QDebuggerServer::onNewConnection()
{
	QTcpSocket* const pSocket = nextPendingConnection();
	m_pConnection = std::make_unique<DebugConnection>(m_pDatabase, pSocket);
	QObject::connect(pSocket, &QTcpSocket::disconnectFromHost, this, &QDebuggerServer::onConnectionClosed);
}

void QDebuggerServer::onConnectionClosed()
{
	m_pConnection.reset();
}

void QDebuggerServer::log(const Debugger::Log& a_log)
{
	if (m_pConnection)
		m_pConnection->log(a_log);
}