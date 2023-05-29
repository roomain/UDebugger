#pragma once
#include <memory>
#include <list>
#include "qdebuggerserver_global.h"
#include <qtcpserver.h>
#include "DebugConnection.h"
#include "DebugDatabase.h"
#include "DebugLog.h"

//class DebugConnection;

class QDEBUGGERSERVER_EXPORT QDebuggerServer : public QTcpServer
{
	Q_OBJECT
public:
	QDebuggerServer();
	~QDebuggerServer() = default;
	void setDatabase(const Debugger::DebugDatabasePtr& a_pDatabase);
	void log(const Debugger::Log& a_log);

private:
	void onNewConnection();
	void onConnectionClosed();
	std::unique_ptr<DebugConnection> m_pConnection;
	Debugger::DebugDatabasePtr m_pDatabase;
};
