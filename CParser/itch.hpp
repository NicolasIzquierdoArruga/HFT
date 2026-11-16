#pragma once              // include guard

#include <cstddef>    // size_t
#include <cstdint>    // uint8_t
#include <istream>    // std::istream 
#include <fstream>

namespace itch {
    // Common header: same position in every message.
    inline constexpr size_t LEN_TYPE      = 1;
    inline constexpr size_t LEN_LOCATE    = 2;
    inline constexpr size_t LEN_TRACKING  = 2;
    inline constexpr size_t LEN_TIMESTAMP = 6;
    inline constexpr size_t LEN_ORDER_REF = 8;   // same in A, E, X, D

    namespace add {          // 'A'
        inline constexpr char TYPE = 'A';
        inline constexpr uint16_t LEN        = 36;
        inline constexpr size_t   LEN_SIDE   = 1;
        inline constexpr size_t   LEN_SHARES = 4;
        inline constexpr size_t   LEN_STOCK  = 8;
        inline constexpr size_t   LEN_PRICE  = 4;
    }

    namespace exe {          // 'E'
        inline constexpr char TYPE = 'E';
        inline constexpr uint16_t LEN           = 31;
        inline constexpr size_t   LEN_EXSHARES  = 4;
        inline constexpr size_t   LEN_MATCHN    = 8;
    }   

    namespace can {          // 'X'
        inline constexpr char TYPE = 'X';
        inline constexpr uint16_t LEN           = 23;
        inline constexpr size_t   LEN_CANCEL    = 4;
    }   

    // Deleted messages don't have special fields
    namespace del {          // 'D'
        inline constexpr char TYPE = 'D';
        inline constexpr uint16_t LEN = 19;
    }
}

// Struct for decoded ITCH messages
struct ItchMessage {
    char message_type           = 0;
    uint16_t stock_locate       = 0;
    uint16_t tracking_number    = 0;
    uint64_t timestamp          = 0;
    uint64_t order_reference    = 0;
    char buy_sell_indicator     = 0; // 'B' for buy, 'S' for sell
    uint32_t shares             = 0;
    char stock[9]               ={}; // 8 characters + null terminator
    uint32_t price              = 0;
    uint64_t match_number       = 0;
};

// 2 bytes, big-endian
inline uint16_t read_be16(const uint8_t* p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}

// 4 bytes, big-endian
inline uint32_t read_be32(const uint8_t* p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}

// 6 bytes, big-endian
inline uint64_t read_be48(const uint8_t* p) {
    return (uint64_t(p[0]) << 40) | (uint64_t(p[1]) << 32) | (uint64_t(p[2]) << 24) | (uint64_t(p[3]) << 16) | 
    (uint64_t(p[4]) << 8) | uint64_t(p[5]);
}

// 8 bytes, big-endian
inline uint64_t read_be64(const uint8_t* p) {
    return (uint64_t(p[0]) << 56) | (uint64_t(p[1]) << 48) | (uint64_t(p[2]) << 40) | (uint64_t(p[3]) << 32) |
           (uint64_t(p[4]) << 24) | (uint64_t(p[5]) << 16) | (uint64_t(p[6]) << 8) | uint64_t(p[7]);
}

bool read_and_check(std::ifstream& file, uint8_t* buf, size_t size);
