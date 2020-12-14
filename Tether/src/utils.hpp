#ifndef UTILS_HPP
#define UTILS_HPP

template<typename SignedShiftableType>
SignedShiftableType signedrshift(const SignedShiftableType& st, int amount)
{
        return ((((st<0)?-1:1) * st) >> amount) * ((st<0)?-1:1);
}

template<typename SignedShiftableType>
SignedShiftableType signedlshift(const SignedShiftableType& st, int amount)
{
        return ((((st<0)?-1:1) * st) << amount) * ((st<0)?-1:1);
}

#endif // UTILS_HPP
