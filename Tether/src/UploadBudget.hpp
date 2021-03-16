#ifndef UPLOADBUDGET_HPP
#define UPLOADBUDGET_HPP

#include <chrono>
#include <algorithm>
#include "WallClock.hpp"

#include "ShortNames.h"

using std::min;
using std::chrono::seconds;

struct UploadBudget
{
    /**
     * @brief maximum of growth for unused budget
     * 
     * This component tracks the number of unused budget when calling used() with a lower number than current().
     * To prevent this number from evermore growing, this maximum value exists.
     */
    i64 max_unused_budget = 10000;

    /**
     * @param timerate ; Default of rate is per second.
     */
    UploadBudget(WallClock& wc, i64 bytes, WallClock::duration timerate = 1s);

    /**
     * @brief Set a new byterate per time
     * 
     * @param timerate ; Default of rate is per second.
     */
    void set(i64 bytes, WallClock::duration timerate = 1s);

    /**
     * @brief Reset internal tracking without changing the byterate.
     * 
     * As this component is time-sensitive, this call provides the functionality of 
     * making the component forget the time before the call.
     */
    void reset();

    /**
     * @brief Get the currently configurated byterate.
     * 
     * @param time ; bytes per this time is calculated
     * @return i64 ; number of bytes in the specified time
     */
    i64 bytesPer(WallClock::duration time) const;

    /**
     * @brief The current number of bytes in the budget.
     */
    i64 current() const;

    /**
     * @brief Notifies this component of the bytes of the budget used.
     */
    void used(i64 usedbytes);

private:
    WallClock& wc;

    i64 bytes;
    WallClock::duration timerate;

    WallClock::time_point last_used;
    i64 unused_budget = 0;

    friend std::ostream& operator<<(std::ostream& os, const UploadBudget& b);
};

#include <iostream>

std::ostream& operator<<(std::ostream& os, const UploadBudget& b);

#endif // UPLOADBUDGET_HPP
