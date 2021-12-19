#pragma once

#include <cstdint>

#include <kpi/cryptomech/lab1/GmpInteger.hpp>

namespace kpi::cryptomech::lab1 {

class GmpTest {
public:
    GmpTest() = default;
    ~GmpTest() = default;

    /**
     * @brief (a + b) % mod
     */
    void testAdd(GmpInteger& a, GmpInteger& b, GmpInteger& mod, const std::uint64_t iterationCount);

    void testSub(GmpInteger& a, GmpInteger& b, GmpInteger& mod, const std::uint64_t iterationCount);

    void testMul(GmpInteger& a, GmpInteger& b, GmpInteger& mod, const std::uint64_t iterationCount);

    void testDiv(GmpInteger& a, GmpInteger& b, GmpInteger& mod, const std::uint64_t iterationCount);

    void testPow(GmpInteger& a, GmpInteger& b, GmpInteger& mod, const std::uint64_t iterationCount);

    void testPowMod(GmpInteger& a,
                    GmpInteger& b,
                    GmpInteger& mod,
                    const std::uint64_t iterationCount);

private:
};

}  // namespace kpi::cryptomech::lab1

#include "impl/GmpTestImpl.hpp"
