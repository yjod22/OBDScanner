#pragma once

#include <cstdint>
#include <functional>
#include <vector>

class CANMessage
{
public:
    CANMessage() : msgCounter(0), stdId(0), extId(0), ide(0), rtr(0), dlc(0), fmi(0) {}
    ~CANMessage() = default;

    // Getters for message fields
    uint16_t getMsgCounter() const { return msgCounter; }
    uint32_t getStdId() const { return stdId; }
    uint32_t getExtId() const { return extId; }
    uint8_t getIde() const { return ide; }
    uint8_t getRtr() const { return rtr; }
    uint8_t getDlc() const { return dlc; }
    const uint8_t* getData() const { return data; }
    uint8_t getFmi() const { return fmi; }

    // Deserialize from a byte array
    void deserialize(const std::vector<uint8_t>& packet)
    {
        size_t offset = 0;
        std::memcpy(&msgCounter, packet.data() + offset, sizeof(msgCounter));
        offset += sizeof(msgCounter);
        std::memcpy(&stdId, packet.data() + offset, sizeof(stdId));
        offset += sizeof(stdId);
        std::memcpy(&extId, packet.data() + offset, sizeof(extId));
        offset += sizeof(extId);
        std::memcpy(&ide, packet.data() + offset, sizeof(ide));
        offset += sizeof(ide);
        std::memcpy(&rtr, packet.data() + offset, sizeof(rtr));
        offset += sizeof(rtr);
        std::memcpy(&dlc, packet.data() + offset, sizeof(dlc));
        offset += sizeof(dlc);
        std::memcpy(data, packet.data() + offset, sizeof(data));
        offset += sizeof(data);
        std::memcpy(fmi, packet.data() + offset, sizeof(fmi));
        offset += sizeof(fmi);
    }

private:
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
