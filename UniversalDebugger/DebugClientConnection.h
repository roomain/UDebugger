#pragma once
#include <list>
#include <qobject.h>
#include <QTcpSocket>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QTimer>
#include "PacketProcessing.h"
#include "defines.h"



class DebugClientConnection : public QObject
{
	Q_OBJECT
private:
	QTcpSocket m_socket;
	QTimer m_timerLog;

	PacketProcessing m_packetProcessor;
	DebugProtocol m_protocol;
	LogsList m_logs;

	void onDataRecieved();
	void onDisconnected();
	void onErrorOccured();
	void decodePacket(const QByteArray& a_packet);
	void onTimerLogs();

public:
	DebugClientConnection(QObject* a_parent = nullptr);
	~DebugClientConnection();
	void connectTo(const QString& a_address, const ushort a_port);
	void onAskTree(const int a_view);
	void onAskProps(const int a_index, const int64_t& a_object);
	void onAskCompare(const int a_index, const int64_t& a_object, const int64_t& a_objectComp);
	void onWriteProp(const int64_t& a_uid, const int a_propIndex, const QVariant& a_value);

signals:
	void sg_connected();
	void sg_disconnected();
	void sg_connectionError(const QString&);
	void sg_list(const QJsonArray&);
	void sg_properties(const int64_t&, const QJsonObject&);
	void sg_remove(const int64_t&);;
	void sg_ownerChange(const int64_t&, const int64_t&);
	void sg_htmlLogs(const QString&);

	void sg_instances(const InstanceList&);
	void sg_variables(const int, const int64_t&, const Debugger::ClassInfo&, const VarList&);
	void sg_compare(const int, const ComparisonData&, const ComparisonData&);
};
