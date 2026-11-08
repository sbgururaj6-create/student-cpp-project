#pragma once
// Common includes and "using" lines shared by every file in the project.
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <iomanip>
#include <numeric>      // std::accumulate
#include <algorithm>    // std::sort, std::find_if, std::copy, std::copy_if, std::remove_copy_if,
                        // std::remove_if, std::stable_partition, std::generate_n
#include <iterator>     // std::back_inserter, std::make_move_iterator, stream iterators
#include <fstream>      // reading / writing files
#include <sstream>      // splitting each line into numbers
#include <cstdlib>      // rand()
#include <ctime>        // seeding rand() with the current time
#include <chrono>       // Timer
#include <stdexcept>    // exceptions: runtime_error, invalid_argument, out_of_range
#include <utility>      // std::move
#include <filesystem>   // building output file names

using std::string;
using std::vector;
using std::list;
using std::deque;
using std::cout;
using std::cin;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::accumulate;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istringstream;
