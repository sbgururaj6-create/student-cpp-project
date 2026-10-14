#pragma once
#include "lib.h"

// A tiny stopwatch. Create it (or call reset()) to start, call elapsed()
// to read how many seconds have passed since then.
class Timer {
    std::chrono::steady_clock::time_point start_;

public:
    Timer() { reset(); }

    void reset() { start_ = std::chrono::steady_clock::now(); }

    double elapsed() const {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count();
    }
};
