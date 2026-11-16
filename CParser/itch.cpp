#include "itch.hpp" 


bool read_and_check(std::ifstream& file, uint8_t* buf, size_t size){
    file.read(reinterpret_cast<char*>(buf), size);
    return static_cast<size_t>(file.gcount()) == size;
}