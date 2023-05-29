#pragma once
#include "debugprotocol_global.h"
#include <functional>
#include <QByteArray>

using DecodeCallback = std::function<void(const QByteArray&)>;

class DEBUGPROTOCOL_EXPORT PacketProcessing
{
private:
	static constexpr uint m_sizeOfSizeT = sizeof(size_t);
	DecodeCallback m_callback;
	QByteArray m_baCache;
	size_t m_packetSize = 0;

	static bool packetSize(const QByteArray& a_packet, size_t& a_packetSize);

public:
	PacketProcessing() = default;
	void setCallback(const DecodeCallback& a_callback);
	void processPacket(const QByteArray& a_packet);
	static void addPacketSize(QByteArray& a_packet);
};

