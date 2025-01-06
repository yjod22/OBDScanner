#pragma once

#include <vector>
#include <cstdint>

struct DecodeResult
{
    enum State { Completed, Ongoing, Error } state;
    std::vector<uint8_t> data;
};

class StreamDecoder
{
public:
    StreamDecoder() = default;
    ~StreamDecoder() = default;
    void reset();
    DecodeResult decode(const std::vector<uint8_t>& stream);

private:
    bool cobsDecode(const std::vector<uint8_t>& encoded, std::vector<uint8_t>& decoded);

    std::vector<uint8_t> buffer_;
    uint32_t streamPtr_{};
};
