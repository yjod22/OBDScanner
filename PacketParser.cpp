#include <cstring>

#include "PacketParser.hpp"

void PacketParser::parse(std::vector<uint8_t>& packet)
{
    CANMessage message{};
    size_t offset{};

    message.msgCounter = toLittleEndian16(packet.data() + offset);
    offset += sizeof(message.msgCounter);

    message.stdId = toLittleEndian32(packet.data() + offset);
    offset += sizeof(message.stdId);

    message.extId = toLittleEndian32(packet.data() + offset);
    offset += sizeof(message.extId);

    message.ide = packet[offset++];
    message.rtr = packet[offset++];
    message.dlc = packet[offset++];

    std::memcpy(&message.data, packet.data() + offset, sizeof(message.data));
    offset += sizeof(message.data);

    message.fmi = packet[offset];

    if(CANMessageCb != nullptr)
    {
        CANMessageCb(message);
    }
}

uint16_t PacketParser::toLittleEndian16(const uint8_t* data) {
    return (static_cast<uint16_t>(data[0]) << 8) | data[1];
}

uint32_t PacketParser::toLittleEndian32(const uint8_t* data) {
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8) |
           data[3];
}
