#pragma once

#include <cstdint>
#include <functional>
#include <vector>

struct CANMessage
{
    uint16_t msgCounter;
    uint32_t stdId;
    uint32_t extId;
    uint8_t ide;
    uint8_t rtr;
    uint8_t dlc;
    uint8_t data[8];
    uint8_t fmi;
};

class PacketParser
{
public:
    PacketParser() = default;
    ~PacketParser() = default;
    void setCANMessageCb(const std::function<void(CANMessage&)> callback) { canMessageCb = callback; }
    void parse(std::vector<uint8_t>& packet);

private:
    std::function<void(CANMessage&)> canMessageCb;
};
