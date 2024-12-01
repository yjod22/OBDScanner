#pragma once

#include <vector>
#include <cstdint>

// Result structure for decode
struct DecodeResult {
    enum State { Completed, Ongoing, Error } state; // Added Error state
    std::vector<uint8_t> data; // Decoded data for Completed state
};

class StreamDecoder {
public:
    void reset();
    DecodeResult decode(const std::vector<uint8_t>& stream);

private:
    std::vector<uint8_t> buffer_; // Buffer for incoming bytes
    static bool cobsDecode(const std::vector<uint8_t>& encoded, std::vector<uint8_t>& decoded);
    uint8_t streamPtr_{};
};
