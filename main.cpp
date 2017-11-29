///////////////////////////
// Tanner Kvarfordt      //
// A02052217             //
// CS 3100 --Dr. Mathias //
// Assignment 4          //
///////////////////////////

#include <iostream>
#include <cstdlib>
#include <queue>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <map>

// Change to larger uint if desired
using _uint_ = uint16_t;

// Size of the sequences to be simulated
const _uint_ SEQUENCE_SIZE = 1000;

// Minimum page id -- MUST BE SMALLER THAN MAX_PAGE_NUM
const _uint_ MIN_PAGE_NUM = 1;

// Maximum page id -- MUST BE LARGER THAN MIN_PAGE_NUM
const _uint_ MAX_PAGE_NUM = 250;

// Minimum number of memory frames to be simulated -- MUST BE SMALLER THAN MAX_FRAMES
const _uint_ MIN_FRAMES = 1;

// Maximum number of memory frames to be simulated -- MUST BE LARGER THAN MIN_FRAMES
const _uint_ MAX_FRAMES = 100;

// The number of frames to step by during the simulation
const _uint_ STEP_BY = 1;

// The number of page sequences to be generated and simulated
const _uint_ NUM_SEQUENCES = 100;

// Brace yourself, this one gets ugly...
//
// At its core, this function generates the needed sequences for the simulation.
//
// The return value, simplified: ptr<map<_uint_, ptr<vector<_uint_>>>>
//      The return value is a pointer to a map:
//          key  : _uint_ identifying the sequence
//          value: pointer to a vector:
//              vector: contains _uint_ entries composing a page sequence
//                  entry: a page id
std::shared_ptr<std::map<_uint_, std::shared_ptr<std::vector<_uint_>>>> generateSequences() {
    std::map<_uint_, std::shared_ptr<std::vector<_uint_>>> sequences;

    std::default_random_engine generator;
    std::uniform_int_distribution<_uint_> distribution(MIN_PAGE_NUM, MAX_PAGE_NUM);

    // Generate NUM_SEQUENCES random sequences, each SEQUENCE_SIZE long
    for (_uint_ i = 0; i < NUM_SEQUENCES; ++i) {
        sequences.insert( std::make_pair(i, std::make_shared<std::vector<_uint_>>(std::vector<_uint_>())) );
        for (_uint_ j = 0; j < SEQUENCE_SIZE; ++j) {
            sequences.at(i)->push_back(distribution(generator));
        }
    }

    return std::make_shared<std::map<_uint_, std::shared_ptr<std::vector<_uint_>>>>(sequences);
}

int main() {
    auto sequences_ptr = generateSequences();

    // See documentation on generateSequence to understand what this variable is
    std::map<_uint_, std::shared_ptr<std::vector<_uint_>>> & sequences = *sequences_ptr;

    for (auto && page : *(sequences.at(0))) {
        std::cout << page << std::endl;
    }

    return EXIT_SUCCESS;
}

