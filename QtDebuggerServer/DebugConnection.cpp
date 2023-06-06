#include "DebugConnection.h"
#include "IDebugObject.h"


DebugConnection::DebugConnection(const Debugger::DebugDatabasePtr& a_pDB, QTcpSocket* const a_pSocket) : 
	m_pSocket{ a_pSocket }, m_pDatabase{ a_pDB }
{
	m_packetProcessor.setCallback(std::bind_front(&DebugConnection::decodePacket, this));
	QObject::connect(m_pSocket.get(), &QTcpSocket::readyRead, this, &DebugConnection::onDataRecieved);

	Debugger::DebugDatabaseReactor reactor
	{
		std::bind_front(&DebugConnection::onObjectAdded, this),
		std::bind_front(&DebugConnection::onObjectRemoved, this),
		std::bind_front(&DebugConnection::onObjectOwner, this)
	};

	if(m_pDatabase)
		m_pDatabase->setReactor(reactor);
}

void DebugConnection::log(const Debugger::Log& a_log)
{
	if (m_pSocket)
	{
		m_pSocket->write(DebugProtocol::genLogPacket(a_log));
		m_pSocket->flush();
	}
}




void DebugConnection::instrumentation(const Debugger::InstrumentationData& a_instrumentation)
{
	if (m_pSocket)
	{
		m_pSocket->write(DebugProtocol::genInstrumentationPacket(a_instrumentation));
		m_pSocket->flush();
	}
}

void DebugConnection::instrumentation(Debugger::InstrumentationData&& a_instrumentation)
{
	if (m_pSocket)
	{
		m_pSocket->write(DebugProtocol::genInstrumentationPacket(a_instrumentation));
		m_pSocket->flush();
	}
}

void DebugConnection::onDataRecieved()
{
	while (m_pSocket->bytesAvailable())
		m_packetProcessor.processPacket(m_pSocket->readAll());
}

void DebugConnection::decodePacket(const QByteArray& a_packet)const
{
	switch (m_protocol.serverRead(a_packet))
	{
	case DebugProtocol::DebugCMDType::cmd_List:
		processList();
		break;

	case DebugProtocol::DebugCMDType::cmd_PropRead:
		processPropertyRead();
		break;

	case DebugProtocol::DebugCMDType::cmd_PropWrite:
		processPropertyWrite();
		break;


	case DebugProtocol::DebugCMDType::cmd_Compare:
		processPropertyCompare();
		break;

	default:
		break;
	}

}

void DebugConnection::processList()const
{
	m_serializer.clear();
	if (m_pSocket && m_pDatabase)
	{
		int view = -1;
		m_protocol.readAskList(view);
		m_pDatabase->serializeTree(m_DBSerializer);
		m_pSocket->write(DebugProtocol::genListPacket(m_DBSerializer, view));
		m_pSocket->flush();
	}
}

void DebugConnection::processPropertyCompare()const
{
	int iView = -1;
	int64_t uid0 = 0;
	int64_t uid1 = 0;
	m_protocol.readReadPropComparePacket(uid0, uid1, iView);
	m_serializer.clear();
	DebugSerializer tmpSerializer;
	if (m_pSocket && m_pDatabase && (uid0 >= 0) && (uid1 >= 0) && 
		m_pDatabase->serializeObject(uid0, m_serializer) &&
		m_pDatabase->serializeObject(uid1, tmpSerializer))
	{
		m_pSocket->write(DebugProtocol::genPropComparePacket(uid0, m_serializer.object(), uid1, tmpSerializer.object(), iView));
		m_pSocket->flush();
	}
}

void DebugConnection::processPropertyRead()const
{
	int iView = -1;
	int64_t uid = 0;
	m_protocol.readReadPropPacket(uid, iView);
	m_serializer.clear();
	if (m_pSocket && m_pDatabase && m_pDatabase->serializeObject(uid, m_serializer))
	{
		m_pSocket->write(DebugProtocol::genPropPacket(m_serializer, iView));
		m_pSocket->flush();
	}
}




void DebugConnection::processPropertyWrite()const
{
	if (m_pSocket && m_pDatabase)
	{
		int64_t uid = 0;
		int index = -1;
		m_protocol.readWritePropPacket(m_deserializer, uid, index);
		if (m_pDatabase->deserializeObject(uid, index, m_deserializer))
		{
			if (m_pDatabase->serializeObject(uid, m_serializer))
			{
				m_pSocket->write(DebugProtocol::genPropPacket(m_serializer, -1));
				m_pSocket->flush();
			}
		}
	}
}

void DebugConnection::onObjectAdded(const int64_t&)const
{
	if (m_pSocket)
		processList();
}

void DebugConnection::onObjectRemoved(const int64_t& a_uid, const int64_t&)const
{
	if (m_pSocket)
	{
		m_pSocket->write(DebugProtocol::genRemovePacket(a_uid));
		m_pSocket->flush();
	}
}

void DebugConnection::onObjectOwner(const int64_t& a_obj, const int64_t& a_owner)const
{
	if (m_pSocket)
	{
		m_pSocket->write(DebugProtocol::genOwnerChangePacket(a_obj, a_owner));
		m_pSocket->flush();
	}
}
