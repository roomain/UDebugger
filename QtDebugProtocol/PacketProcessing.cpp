#include "PacketProcessing.h"

bool PacketProcessing::packetSize(const QByteArray& a_packet, size_t& a_packetSize)
{
	a_packetSize = 0;
	bool bRet = a_packet.size() >= m_sizeOfSizeT;
	if (bRet)
	{
		const size_t* tmp = reinterpret_cast<const size_t*>(a_packet.constData());
		a_packetSize = *tmp;
	}
	return bRet;
}

void PacketProcessing::addPacketSize(QByteArray& a_packet)
{
	size_t packetSize = a_packet.size();
	a_packet.prepend(reinterpret_cast<const char*>(&packetSize), m_sizeOfSizeT);
}

void PacketProcessing::setCallback(const DecodeCallback& a_callback)
{
	m_callback = a_callback;
}

void PacketProcessing::processPacket(const QByteArray& a_packet)
{
	if (m_baCache.isEmpty() && PacketProcessing::packetSize(a_packet, m_packetSize))
	{
		// new packet
		m_baCache = a_packet.right(a_packet.size() - m_sizeOfSizeT);
	}
	else
	{
		// complete packet
		m_baCache.append(a_packet.left(std::min<size_t>(a_packet.size(), m_sizeOfSizeT)));
	}

	while (m_baCache.size() >= m_packetSize && m_packetSize > 0)
	{
		if (m_callback)
			m_callback(m_baCache.right(m_packetSize));

		m_baCache = m_baCache.left(m_baCache.size() - m_packetSize);

		if(PacketProcessing::packetSize(m_baCache, m_packetSize))
			m_baCache = a_packet.right(m_baCache.size() - m_sizeOfSizeT);

	}
}