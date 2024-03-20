#ifndef BLOCK_INFO_H
#define BLOCK_INFO_H

#include <cstdint>

#include <string>

#include "common_types.h"

struct BlockInfo {
    uint256 hash;
    uint32_t timestamp{0};
    uint256 challenge;
    int height{0};
    int filter_bits{0};
    uint64_t block_difficulty{0};
    uint64_t challenge_difficulty{0};
    Bytes farmer_pk;
    std::string address;
    double reward{0};
    uint64_t accumulate{0};
    std::string vdf_time;
    int vdf_iters{0};
    int vdf_speed{0};
    int vdf_iters_req{0};
    uint64_t vdf_size{0};
};

#endif
