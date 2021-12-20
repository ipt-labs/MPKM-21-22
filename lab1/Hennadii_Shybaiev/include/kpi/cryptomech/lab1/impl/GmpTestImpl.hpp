#pragma once

#include <chrono>
#include <cstdint>

#include <spdlog/spdlog.h>

#include <kpi/cryptomech/lab1/GmpInteger.hpp>
#include <kpi/cryptomech/lab1/GmpTest.hpp>

namespace kpi::cryptomech::lab1 {

void GmpTest::testAdd(GmpInteger& a,
                      GmpInteger& b,
                      GmpInteger& mod,
                      const std::uint64_t iterationCount)
{
    using namespace std::chrono;

    auto start = system_clock::now();
    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.add(b.getGmpValue()).mod(mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("Add test: {} in ms", diff.count());
}

void GmpTest::testSub(GmpInteger& a,
                      GmpInteger& b,
                      GmpInteger& mod,
                      const std::uint64_t iterationCount)
{
    using namespace std::chrono;
    auto start = system_clock::now();
    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.sub(b.getGmpValue()).mod(mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("Sub test: {} in ms", diff.count());
}

void GmpTest::testMul(GmpInteger& a,
                      GmpInteger& b,
                      GmpInteger& mod,
                      const std::uint64_t iterationCount)
{
    using namespace std::chrono;
    auto start = system_clock::now();
    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.mul(b.getGmpValue()).mod(mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("Mul test: {} in ms", diff.count());
}

void GmpTest::testDiv(GmpInteger& a,
                      GmpInteger& b,
                      GmpInteger& mod,
                      const std::uint64_t iterationCount)
{
    using namespace std::chrono;
    auto start = system_clock::now();
    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.div(b.getGmpValue()).mod(mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("Div test: {} in ms", diff.count());
}

void GmpTest::testPow(GmpInteger& a,
                      GmpInteger& b,
                      GmpInteger& mod,
                      const std::uint64_t iterationCount)
{
    using namespace std::chrono;
    auto start = system_clock::now();

    if (not b.getStdValue()) {
        return;
    }

    std::uint64_t bValue = *b.getStdValue();

    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.pow(bValue).mod(mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("Pow test: {} in ms", diff.count());
}

void GmpTest::testPowMod(GmpInteger& a,
                         GmpInteger& b,
                         GmpInteger& mod,
                         const std::uint64_t iterationCount)
{
    using namespace std::chrono;
    auto start = system_clock::now();
    for (std::uint64_t ix = 0; ix < iterationCount; ++ix) {
        a.powMod(2, mod.getGmpValue());
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    spdlog::info("PowMod test: {} in ms", diff.count());
}

}  // namespace kpi::cryptomech::lab1
