#include "UploadBudget.hpp"

std::ostream& operator<<(std::ostream& os, const UploadBudget& b)
{
    return os   << '['
                << b.bytes << " / " << b.timerate.count() << "us ;"
                << "last used: " << b.last_used.time_since_epoch().count()
                << "unused_budget : "<< b.unused_budget 
                << ']';
}

UploadBudget::UploadBudget(WallClock& wc, i64 bytes, WallClock::duration timerate)
    : wc(wc)
    , bytes(bytes), timerate(timerate)
    , last_used(wc.now())
    , unused_budget(0)
{}

void UploadBudget::set(i64 bytes, WallClock::duration timerate)
{
    bytes = bytes; timerate = timerate;
    reset();
}

void UploadBudget::reset() { last_used = wc.now(); unused_budget = 0; }
i64 UploadBudget::bytesPer(WallClock::duration time) const 
{ 
    return bytes * ((double)time.count() / (double)timerate.count());
}

i64 UploadBudget::current() const 
{ 
    return bytesPer(wc.now() - last_used) + unused_budget; 
}

void UploadBudget::used(i64 usedbytes)
{
    unused_budget = min(current() - usedbytes, max_unused_budget);
    last_used = wc.now();
}