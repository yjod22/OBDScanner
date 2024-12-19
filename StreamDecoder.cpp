#include "StreamDecoder.hpp"

void StreamDecoder::reset()
{
    streamPtr_ = 0;
}

// StreamDecoder's decode implementation
DecodeResult StreamDecoder::decode(const std::vector<uint8_t>& stream)
{
    for(; streamPtr_ < stream.size() ; streamPtr_++)
    {
        if (stream[streamPtr_] == 0x00)
        {
            std::vector<uint8_t> decodedStream;
            if (cobsDecode(buffer_, decodedStream))
            {
                buffer_.clear();
                streamPtr_++;
                return {DecodeResult::Completed, decodedStream};
            }
            else
            {
                buffer_.clear();
                return {DecodeResult::Error, {}};
            }
        }
        else
        {
            buffer_.push_back(stream[streamPtr_]);
        }
    }

    return {DecodeResult::Ongoing, {}};
}

bool StreamDecoder::cobsDecode(const std::vector<uint8_t>& encoded, std::vector<uint8_t>& decoded) {
    if (encoded.empty())
    {
        return false;
    }

    decoded.clear();
    size_t index = 0;
    size_t length = encoded.size();

    while (index < length) {
        uint8_t code = encoded[index++];
        if (code == 0 || index + code - 1 > length)
        {
            // Invalid COBS encoding
            return false;
        }

        for (uint8_t i = 1; i < code; ++i)
        {
            decoded.push_back(encoded[index++]);
        }

        if (code < 0xFF && index < length)
        {
            // Reinsert delimiter
            decoded.push_back(0x00);
        }
    }

    return true;
}

