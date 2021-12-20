#pragma once

#include <iostream>
#include <optional>
#include <string>

#include <gmp.h>

namespace kpi::cryptomech::lab1 {

class GmpInteger {
public:
    GmpInteger();

    GmpInteger(int initValue);

    GmpInteger(const GmpInteger&) = delete;

    ~GmpInteger();

    void operator=(int rOperand);

    mpz_t& operator+(mpz_t rOperand);

    mpz_t& operator+=(mpz_t rOperand);

    mpz_t& operator+=(int rOperand);

    mpz_t& operator-(mpz_t rOperand);

    mpz_t& operator-=(mpz_t rOperand);

    GmpInteger& add(const mpz_t value);

    GmpInteger& sub(const mpz_t value);

    GmpInteger& sub(const std::uint64_t value);

    GmpInteger& mod(const mpz_t modulVal);

    GmpInteger& mul(const mpz_t value);

    GmpInteger& div(const mpz_t value);

    GmpInteger& pow(const std::uint32_t powValue);

    GmpInteger& powMod(const mpz_t exp, const mpz_t mod);

    GmpInteger& powMod(const std::uint64_t exp, const mpz_t mod);

    static void powMod(mpz_t& result, const mpz_t base, const mpz_t exp, const mpz_t mod);

    const mpz_t& getGmpValue();

    std::optional<int> getStdValue();

private:
    mpz_t mValue;
};

}  // namespace kpi::cryptomech::lab1

#include "impl/GmpIntegerImpl.hpp"
