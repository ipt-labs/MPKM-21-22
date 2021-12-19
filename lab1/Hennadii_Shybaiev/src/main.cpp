#include <cstdint>
#include <iostream>

#include "gmp.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <kpi/cryptomech/lab1/GmpInteger.hpp>
#include <kpi/cryptomech/lab1/GmpTest.hpp>

using kpi::cryptomech::lab1::GmpInteger;
using kpi::cryptomech::lab1::GmpTest;

namespace {

const std::uint64_t iterationCount{1'000'000};

}  // namespace

int main()
{
    std::shared_ptr<spdlog::logger> stdout_logger = spdlog::stdout_color_mt("stdout");
    spdlog::set_default_logger(stdout_logger);

    // Test for 256:
    {
        spdlog::info("Test 256:");

        GmpInteger a256(2);
        a256.pow(256).sub(50'000'000'000);

        GmpInteger b256(2);
        b256.pow(256).sub(10'000'000'000);

        GmpInteger mod(2);
        mod.pow(256);

        GmpTest test;
        test.testAdd(a256, b256, mod, iterationCount);
        test.testSub(a256, b256, mod, iterationCount);
        test.testMul(a256, b256, mod, iterationCount);
        test.testDiv(a256, b256, mod, iterationCount);
        test.testPow(a256, b256, mod, iterationCount);
        test.testPowMod(a256, b256, mod, iterationCount);
    }

    {
        spdlog::info("Test 512:");

        GmpInteger a512(2);
        a512.pow(512).sub(50'000'000'000);

        GmpInteger b512(2);
        b512.pow(512).sub(10'000'000'000);

        GmpInteger mod(2);
        mod.pow(512);

        GmpTest test;
        test.testAdd(a512, b512, mod, iterationCount);
        test.testSub(a512, b512, mod, iterationCount);
        test.testMul(a512, b512, mod, iterationCount);
        test.testDiv(a512, b512, mod, iterationCount);
        test.testPow(a512, b512, mod, iterationCount);
        test.testPowMod(a512, b512, mod, iterationCount);
    }

    {
        spdlog::info("Test 1024:");

        GmpInteger a1024(2);
        a1024.pow(1024).sub(50'000'000'000);

        GmpInteger b1024(2);
        b1024.pow(1024).sub(10'000'000'000);

        GmpInteger mod(2);
        mod.pow(1024);

        GmpTest test;
        test.testAdd(a1024, b1024, mod, iterationCount);
        test.testSub(a1024, b1024, mod, iterationCount);
        test.testMul(a1024, b1024, mod, iterationCount);
        test.testDiv(a1024, b1024, mod, iterationCount);
        test.testPow(a1024, b1024, mod, iterationCount);
        test.testPowMod(a1024, b1024, mod, iterationCount);
    }

    return 0;
}
