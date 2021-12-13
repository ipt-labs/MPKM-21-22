#pragma once
#include "MillerRabin.h"
#include <sstream>
#include <iomanip>

Integer mulmod(Integer a, Integer b, Integer mod)
{
    Integer x = 0, y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
            x = (x + y) % mod;
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

bool miller_rabin(Integer p, int iteration)
{
    if (p < 2)
        return false;
    if (p != 2 && p % 2 == 0)
        return false;
    Integer s = p - 1;
    while (s % 2 == 0)
        s /= 2;
    for (int i = 0; i < iteration; i++)
    {
        Integer a = rand() % (p - 1) + 1, temp = s;
        Integer mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
            return false;
    }
    return true;
}
