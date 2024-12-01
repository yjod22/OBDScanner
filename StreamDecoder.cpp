#include "StreamDecoder.h"

void StreamDecoder::reset()
{
    streamPtr_ = 0;
}

// StreamDecoder's decode implementation
DecodeResult StreamDecoder::decode(const std::vector<uint8_t>& stream) {
    for(; streamPtr_ < stream.size() ; streamPtr_++)
    {
        if (stream[streamPtr_] == 0x00)
        {
            // Found delimiter, attempt to decode
            std::vector<uint8_t> decodedPacket;
            if (cobsDecode(buffer_, decodedPacket))
            {
                buffer_.clear(); // Clear buffer for the next packet
                streamPtr_++;
                return {DecodeResult::Completed, decodedPacket};
            }
            else
            {
                // Malformed packet
                buffer_.clear(); // Clear buffer to recover
                return {DecodeResult::Error, {}};
            }
        }
        else
        {
            // Accumulate data into buffer
            buffer_.push_back(stream[streamPtr_]);
        }
    }

    // No complete packet found yet
    return {DecodeResult::Ongoing, {}};
}


// COBS decoding implementation
bool StreamDecoder::cobsDecode(const std::vector<uint8_t>& encoded, std::vector<uint8_t>& decoded) {
    if (encoded.empty()) {
        return false; // Invalid input: empty data
    }

    decoded.clear();
    size_t index = 0;
    size_t length = encoded.size();

    while (index < length) {
        uint8_t code = encoded[index++];
        if (code == 0 || index + code - 1 > length) {
            return false; // Invalid COBS encoding
        }

        for (uint8_t i = 1; i < code; ++i) {
            decoded.push_back(encoded[index++]);
        }

        if (code < 0xFF && index < length) {
            decoded.push_back(0x00); // Reinsert delimiter
        }
    }

    return true;
}

