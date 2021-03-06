///////////////////////////
// Tanner Kvarfordt      //
// A02052217             //
// CS 3100 --Dr. Mathias //
// Assignment 6          //
///////////////////////////

#include <iostream>
#include <cstdlib>
#include <queue>
#include <memory>
#include <vector>
#include <random>
#include <map>
#include <array>
#include <chrono>

// Change to larger uint if desired
using _uint_ = uint16_t;

// Size of the sequences to be simulated
const _uint_ SEQUENCE_SIZE = 1000;

// Minimum page id -- DO NOT CHANGE; WILL BREAK test() FUNCTION
// Should always be 1
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
// The return value, simplified: ptr<vector<ptr<vector<_uint_>>>>
//      The return value is a pointer to a vector:
//          entry: pointer to a vector:
//              vector: contains _uint_ entries composing a page sequence
//                  entry: a page id
std::shared_ptr<std::vector<std::shared_ptr<std::vector<_uint_>>>> generateSequences() {
    std::vector<std::shared_ptr<std::vector<_uint_>>> sequences;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<_uint_> distribution(MIN_PAGE_NUM, MAX_PAGE_NUM);

    // Generate NUM_SEQUENCES random sequences, each SEQUENCE_SIZE long
    for (_uint_ i = 0; i < NUM_SEQUENCES; ++i) {
        sequences.push_back(std::make_shared<std::vector<_uint_>>(std::vector<_uint_>()));
        for (_uint_ j = 0; j < SEQUENCE_SIZE; ++j) {
            sequences.at(i)->push_back(distribution(generator));
        }
    }

    return std::make_shared<std::vector<std::shared_ptr<std::vector<_uint_>>>>(sequences);
}

// Tests a FIFO queue page replacement scheme for Belady's Anomaly using @num_frames memory frames and the provided @sequence
// Returns the number of page faults that occurred
_uint_ test(_uint_ const & num_frames, std::vector<_uint_> const & sequence) {

    // Tracks the pages that are loaded into memory
    // Since MIN_PAGE_NUM is 1, any access must be offset by 1
    // e.g. (memory[1] == true) means that page 2 is loaded in memory
    std::array<bool, MAX_PAGE_NUM>  memory;
    memory.fill(false);

    // FIFO queue tracking oldest page in memory
    std::queue<_uint_> queue;

    // Number of page faults detected
    _uint_ pageFaults = 0;

    // Number of pages loaded into memory
    _uint_ loadedPages = 0;

    for (auto && page : sequence) {
        if (!memory.at(page - 1)) {
            ++pageFaults;
            if (loadedPages < num_frames) {
                ++loadedPages;
                memory[page - 1] = true;
                queue.push(page);
            }
            else {
                memory[queue.front() - 1] = false;
                queue.pop();
                memory[page - 1] = true;
                queue.push(page);
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
    auto start = std::chrono::system_clock::now();

    std::cout << "Length of memory reference string: " << SEQUENCE_SIZE << std::endl
              << "Frames of physical memory: " << MAX_FRAMES << std::endl << std::endl;

    auto sequences_ptr = generateSequences();

    if (sequences_ptr == nullptr) {
        std::cout << "ERROR! nullptr" << std::endl;
        return EXIT_FAILURE;
    }

    // See documentation on generateSequence to understand what this variable is
    std::vector<std::shared_ptr<std::vector<_uint_>>> & sequences = *sequences_ptr;


    // Contains the number of page faults associated with each run of each sequence
    // Ex: pageFaults[i][j] contains the number of page faults for the i'th sequence using j frames of memory
    std::vector<std::map<_uint_, _uint_>> pageFaults(NUM_SEQUENCES);

    // The number of times Belady's Anomaly is detected
    _uint_ anomalyDetected = 0;

    // Test each sequence from MIN_FRAMES to MAX_FRAMES, stepping by STEP_BY frames per test
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

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Program execution time: " << elapsed_seconds.count() << " seconds" << std::endl;

    return EXIT_SUCCESS;
}

