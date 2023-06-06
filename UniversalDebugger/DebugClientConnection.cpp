#include "DebugClientConnection.h"
#include "DebugProperties.h"
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonvalue.h>
#include <qfileinfo.h>

void DebugClientConnection::onDataRecieved()
{
	while (m_socket.bytesAvailable())
		m_packetProcessor.processPacket(m_socket.readAll());
}


void DebugClientConnection::decodePacket(const QByteArray& a_packet)
{
	switch (m_protocol.clientRead(a_packet))
	{
	case DebugProtocol::DebugAnsType::ans_List:
		{
			int iViewId;
			InstanceList lInstance;
			m_protocol.readListPacket(lInstance, iViewId);
			emit sg_instances(lInstance);
		}
		break;

	case DebugProtocol::DebugAnsType::ans_OwnerChange:
	{
		int64_t uuid = -1;
		int64_t owner = -1;
		m_protocol.readOwnerChangePacket(uuid, owner);
		emit sg_ownerChange(uuid, owner);
	}
		break;

	case DebugProtocol::DebugAnsType::ans_Instrument:
		// TODO
		break;

	case DebugProtocol::DebugAnsType::ans_Log:
	{
		DebugProtocol::LogMessage msg;
		m_protocol.readLogPacket(msg);
		m_logs.emplace_back(msg);
	}
		break;

	case DebugProtocol::DebugAnsType::ans_ObjRemoved:
		{
			int64_t uuid = -1;
			m_protocol.readRemovePacket(uuid);
			emit sg_remove(uuid);
		}
		break;

	case DebugProtocol::DebugAnsType::ans_PropRead:
	case DebugProtocol::DebugAnsType::ans_PropWrite:
		{
			int iViewId;
			int64_t uuid;
			VarList vProp;
			Debugger::ClassInfo info;
			m_protocol.readPropPacket(uuid, info, vProp, iViewId);
			emit sg_variables(iViewId, uuid, info, vProp);
		}
		break;


	case DebugProtocol::DebugAnsType::ans_Compare:
	{
		int iViewId;
		ComparisonData data0;
		ComparisonData data1;
		m_protocol.reaPropComparePacket(data0, data1, iViewId);
		emit sg_compare(iViewId, data0, data1);
	}
		break;

	default:
		break;
	}
	
}

void DebugClientConnection::onErrorOccured()
{
	emit sg_connectionError(m_socket.errorString());
}

void DebugClientConnection::onDisconnected()
{
	emit sg_connectionError(m_socket.errorString());
}

DebugClientConnection::DebugClientConnection(QObject* a_parent) : QObject(a_parent), m_socket(this), m_timerLog(this)
{
	m_timerLog.setInterval(1000);
	QObject::connect(&m_timerLog, &QTimer::timeout, this, &DebugClientConnection::onTimerLogs);
	QObject::connect(&m_socket, &QTcpSocket::connected, this, &DebugClientConnection::sg_connected); 
	QObject::connect(&m_socket, &QTcpSocket::disconnected, this, &DebugClientConnection::onDisconnected);
	QObject::connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(& QTcpSocket::errorOccurred), this, &DebugClientConnection::onErrorOccured);

	QObject::connect(&m_socket, &QTcpSocket::readyRead, this, &DebugClientConnection::onDataRecieved);
	m_packetProcessor.setCallback(std::bind_front(&DebugClientConnection::decodePacket, this));
	m_timerLog.start();
}

DebugClientConnection::~DebugClientConnection()
{
	m_socket.disconnectFromHost();
}

void DebugClientConnection::onTimerLogs()
{
	QString html;
	QString sLog;
	for (auto&& log : m_logs)
	{
		QFileInfo fInfo(log.m_sFileLocation);
		sLog = QString("[%1] %2 %3 L%4 - %5").arg(log.m_sTime).arg(fInfo.fileName())
			.arg(log.m_sFunctionLocation).arg(log.m_iLineLocation).arg(log.m_logMessage);
		switch (log.m_severity)
		{
		case Debugger::LogSeverity::log_critical:
		case Debugger::LogSeverity::log_error:
		case Debugger::LogSeverity::log_exception:
			html += QString("<p><span style=\"color:#FF0000;font-weight:bold;\">%1</span><br/></p>").arg(sLog);
			break;
		case Debugger::LogSeverity::log_warning:
			html += QString("<p><span style=\"color:#FF6A00;\">%1</span><br/></p>").arg(sLog);
			break;
		case Debugger::LogSeverity::log_info:
		default:
			html += QString("<p><span style=\"color:#000000;\">%1</span><br/></p>").arg(sLog);
			break;
		}
	}
	m_logs.clear();

	emit sg_htmlLogs(html);
}


void DebugClientConnection::connectTo(const QString& a_address, const ushort a_port)
{
	m_socket.disconnectFromHost();
	m_socket.connectToHost(a_address, a_port);
}


void DebugClientConnection::onAskTree(const int a_view)
{
	m_socket.write(m_protocol.askList(a_view));
	m_socket.flush();
}

void DebugClientConnection::onAskProps(const int a_index, const int64_t& a_object)
{
	m_socket.write(m_protocol.genReadPropPacket(a_object, a_index));
	m_socket.flush();
}

void DebugClientConnection::onAskCompare(const int a_index, const int64_t& a_object, const int64_t& a_objectComp)
{
	m_socket.write(m_protocol.genReadPropComparePacket(a_object, a_objectComp, a_index));
	m_socket.flush();
}

void DebugClientConnection::onWriteProp(const int64_t& a_uid, const int a_propIndex, const QVariant& a_value)
{
	m_socket.write(m_protocol.genWritePropPacket(a_uid, a_propIndex, a_value));
	m_socket.flush();
}
