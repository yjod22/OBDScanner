#include <cstring>

#include "PacketParser.hpp"

void PacketParser::parse(std::vector<uint8_t> &packet)
{
    CANMessage message{};
    size_t offset{};

    std::memcpy(&message.msgCounter, packet.data() + offset, sizeof(message.msgCounter));
    offset += sizeof(message.msgCounter);

    std::memcpy(&message.stdId, packet.data() + offset, sizeof(message.stdId));
    offset += sizeof(message.stdId);

    std::memcpy(&message.extId, packet.data() + offset, sizeof(message.extId));
    offset += sizeof(message.extId);

    message.ide = packet[offset++];
    message.rtr = packet[offset++];
    message.dlc = packet[offset++];

    std::memcpy(&message.data, packet.data() + offset, sizeof(message.data));
    offset += sizeof(message.data);

    message.fmi = packet[offset];

    if (canMessageCb != nullptr)
    {
        canMessageCb(message);
    }
}
