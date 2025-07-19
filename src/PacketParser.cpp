#include <cstring>

#include "PacketParser.hpp"

void PacketParser::parse(std::vector<uint8_t>& packet)
{
    CANMessage message{};
    message.deserialize(packet);

    if (canMessageCb != nullptr)
    {
        canMessageCb(message);
    }
}
