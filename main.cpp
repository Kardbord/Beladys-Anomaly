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
#include <unordered_set>
#include <chrono>

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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
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

// Tests a FIFO queue page replacement scheme for Belady's Anomaly using @num_frames memory frames and the provided @sequence
// Returns the number of page faults that occurred
_uint_ test(_uint_ const & num_frames, std::vector<_uint_> const & sequence) {

    // Tracks the pages that are loaded into memory
    std::unordered_set<_uint_> memory;

    // FIFO queue tracking oldest page
    std::queue<_uint_> queue;

    // Number of page faults detected
    _uint_ pageFaults;

    for (auto && page : sequence) {
        if (memory.find(page) == memory.end()) {
            ++pageFaults;
            if (memory.size() < num_frames) {
                memory.insert(page);
                queue.push(page);
            }
            else {
                memory.erase(queue.front());
                queue.pop();
            }
        }
    }

    return pageFaults;
}

// Prints an informative message when Belady's Anomaly is detected
//
// @sequence   : The sequence id being tested
// @frames1    : The smaller number of frames (first test)
// @frames2    : The larger number of frames (second test) 
// @pageFaults1: The smaller number of page faults (first test) 
// @pageFaults2: The larger number of page faults (second test) 
void printAnomaly(_uint_ const & sequence, _uint_ const & frames1, _uint_ const & frames2,
                  _uint_ const & pageFaults1, _uint_ const & pageFaults2) {
    std::cout << "Anomaly Discovered!" << std::endl;
    std::cout << "    Sequence: " << sequence << std::endl
              << "    Page Faults: " << pageFaults1 << " @ Frame Size: " << frames1 << std::endl
              << "    Page Faults: " << pageFaults2 << " @ Frame Size: " << frames2 << std::endl;
}

int main() {
    std::cout << "Length of memory reference string: " << SEQUENCE_SIZE << std::endl
              << "Frames of physical memory: " << MAX_FRAMES << std::endl << std::endl;

    auto sequences_ptr = generateSequences();

    if (sequences_ptr == nullptr) {
        std::cout << "ERROR! nullptr" << std::endl;
        return EXIT_FAILURE;
    }

    // See documentation on generateSequence to understand what this variable is
    std::map<_uint_, std::shared_ptr<std::vector<_uint_>>> & sequences = *sequences_ptr;


    // Contains the number of page faults associated with each run of each sequence
    // Ex: pageFaults[i][j] contains the number of page faults for the i'th sequence using j frames of memory
    std::vector<std::map<_uint_, _uint_>> pageFaults(NUM_SEQUENCES);

    // The number of times Belady's Anomaly is detected
    _uint_ anomalyDetected = 0;

    // Test each sequence from MIN_FRAMES to MAX_FRAMES, steppin by STEP_BY frames per test
    for (_uint_ i = 0; i < NUM_SEQUENCES; ++i) {
        bool firstRun = true;
        for (_uint_ j = MIN_FRAMES; j <= MAX_FRAMES; j += STEP_BY) {
            pageFaults[i][j] = test(j, *sequences.at(i));
            if (!firstRun && pageFaults[i][j] > pageFaults[i][j - STEP_BY]) {
                // Anomaly detected
                ++anomalyDetected;
                printAnomaly(i + 1, j - STEP_BY, j, pageFaults[i][j - STEP_BY], pageFaults[i][j]);
            } else if (firstRun) firstRun = false;
        }
    }

    std::cout << std::endl << "Anomaly detected " << anomalyDetected << " times." << std::endl;

    return EXIT_SUCCESS;
}

