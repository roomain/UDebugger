#pragma once
#include <string_view>
#include <qobject.h>
#include <qtcpsocket.h>
#include <qbytearray.h>
#include <memory>
#include <DebugDatabase.h>
#include <qjsonobject.h>
#include "DebugSerializer.h"
#include "DebugDeserializer.h"
#include "DebugInstrumentation.h"
#include "PacketProcessing.h"
#include "DebugProtocol.h"
#include "DebugLog.h"
#include "DebugDatabase.h"

class DebugConnection : public QObject
{
    Q_OBJECT
private:
    std::unique_ptr<QTcpSocket> m_pSocket;
    PacketProcessing m_packetProcessor;
    Debugger::DebugDatabasePtr m_pDatabase;
    mutable DebugProtocol m_protocol;
    mutable DebugDBSerializer m_DBSerializer;
    mutable DebugSerializer m_serializer;
    mutable DebugDeserializer m_deserializer;


public:
    DebugConnection() = delete;
    explicit DebugConnection(const Debugger::DebugDatabasePtr& a_pDB, QTcpSocket* const a_pSocket);
    ~DebugConnection() = default;
    void log(const Debugger::Log& a_log);
    void instrumentation(const Debugger::InstrumentationData& a_instrumentation);
    void instrumentation(Debugger::InstrumentationData&& a_instrumentation);

private:
    void onDataRecieved();
    void decodePacket(const QByteArray& a_packet)const;
    void processList()const;
    void processPropertyRead()const;
    void processPropertyWrite()const;

    void onObjectAdded([[maybe_unused]] const int64_t& a_uid)const;
    void onObjectRemoved([[maybe_unused]] const int64_t& a_uid, [[maybe_unused]] const int64_t& a_parentUID)const;
    void onObjectOwner([[maybe_unused]] const int64_t& a_uid, const int64_t& a_parentUID)const;
};

