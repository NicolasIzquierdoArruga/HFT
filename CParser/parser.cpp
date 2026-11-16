#include <cstdint>
#include <fstream>
#include <iostream>
#include "itch.hpp"

int main() {
    // Converts the file into an object
    std::ifstream file("data/itch_100B.bin", std::ios::binary);

    // Handles error if the file cannot be opened
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Buffer to hold bytes read from the file. Big enough for any message.
    uint8_t buf[64];


    size_t n_messages   = 0;
    size_t n_skipped    = 0;
    uint16_t msg_len    = 0;
    uint8_t  msg_type   = 0;
    uint16_t msg_locat  = 0;
    uint16_t msg_track  = 0;
    uint64_t msg_times  = 0;
    uint64_t msg_orefn  = 0;
    uint8_t  msg_side   = 0;
    uint32_t msg_shar   = 0;
    uint64_t msg_stock  = 0;
    uint32_t msg_price  = 0;
    uint32_t msg_exsha  = 0;
    uint64_t msg_match  = 0;
    uint32_t msg_cance  = 0;
    

    // Each message is: [2-byte big-endian length][message body of that length].
    // The loop stops as soon as a read can't be completed (EOF or error).
    while (read_and_check(file, buf, 2)) {

        msg_len = read_be16(buf);

        if (msg_len == 0){
            std::cerr << "Invalid length after "
                      << n_messages + n_skipped << " messages"
                      << std::endl;
            return 1;
        } 

        // Reinitialize all msg
        msg_type   = 0;
        msg_locat  = 0;
        msg_track  = 0;
        msg_times  = 0;
        msg_orefn  = 0;
        msg_side   = 0;
        msg_shar   = 0;
        msg_stock  = 0;
        msg_price  = 0;
        msg_exsha  = 0;
        msg_match  = 0;
        msg_cance  = 0;


        
        // Start reading
        read_and_check(file, buf, itch::LEN_TYPE);
        msg_type = *buf;

        if (msg_type != itch::add::TYPE && msg_type != itch::exe::TYPE &&
            msg_type != itch::del::TYPE && msg_type != itch::can::TYPE){

                file.ignore(msg_len - itch::LEN_TYPE);
                if (static_cast<size_t>(file.gcount()) != msg_len - itch::LEN_TYPE) {
                    std::cout << "Truncated message at end of file after "
                              << n_messages + n_skipped << " messages." << std::endl;
                    break;
                }
                n_skipped++;
                continue;
            }

        read_and_check(file, buf, itch::LEN_LOCATE);
        msg_locat = read_be16(buf);

        read_and_check(file, buf, itch::LEN_TRACKING);
        msg_track = read_be16(buf);
        
        read_and_check(file, buf, itch::LEN_TIMESTAMP);
        msg_times = read_be48(buf);

        read_and_check(file, buf, itch::LEN_ORDER_REF);
        msg_orefn = read_be64(buf);
        switch (msg_type){
            case itch::add::TYPE :  read_and_check(file, buf, itch::add::LEN_SIDE);
                                    msg_side = *buf;
                                    read_and_check(file, buf, itch::add::LEN_SHARES);
                                    msg_shar = read_be32(buf);
                                    read_and_check(file, buf, itch::add::LEN_STOCK);
                                    msg_stock = read_be64(buf);
                                    read_and_check(file, buf, itch::add::LEN_PRICE);
                                    msg_price = read_be32(buf);
                                    break;

            case itch::exe::TYPE :  read_and_check(file, buf, itch::exe::LEN_EXSHARES);
                                    msg_exsha = read_be32(buf);
                                    read_and_check(file, buf, itch::exe::LEN_MATCHN);
                                    msg_match = read_be64(buf);
                                    break;

            case itch::can::TYPE :  read_and_check(file, buf, itch::can::LEN_CANCEL);
                                    msg_cance = read_be32(buf);
                                    break;
                                            
            default:                break;            
        }

        // Distinguish a clean EOF from a truncated message / I/O error
        if (file.bad()) {
            std::cerr << "I/O error after " << n_messages + n_skipped<< " messages." << std::endl;
            return 1;
        }
        if (file.fail()) {
            std::cerr << "Truncated message at end of file after "
                      << n_messages + n_skipped<< " messages." << std::endl;
            break;
        }

        n_messages++;
    }


    std::cout   << "Parsed " << n_messages << " messages.\n" 
                << "Skipped " << n_skipped  << " messages.\n"
                << std::endl;
    return 0;

}