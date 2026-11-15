#pragma once              // include guard

#include <cstddef>    // size_t
#include <cstdint>    // uint8_t
#include <istream>    // std::istream 
#include <fstream>

// Struct for decoded ITCH messages
struct ItchMessage {
    uint8_t message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference;
    char buy_sell_indicator; // 'B' for buy, 'S' for sell
    uint32_t shares;
    char stock[9]; // 8 characters + null terminator
    uint32_t price;
    uint64_t match_number;
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
