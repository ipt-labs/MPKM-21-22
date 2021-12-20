#pragma once

#include <optional>
#include <string>

#include <gmp.h>

#include <kpi/cryptomech/lab1/GmpInteger.hpp>

namespace kpi::cryptomech::lab1 {

GmpInteger::GmpInteger()
{
    mpz_init(mValue);
}

GmpInteger::GmpInteger(int initValue)
{
    mpz_init(mValue);
    mpz_set_ui(mValue, initValue);
}

GmpInteger::~GmpInteger()
{
    mpz_clear(mValue);
}

void GmpInteger::operator=(int rOperand)
{
    mpz_set_ui(mValue, rOperand);
}

mpz_t& GmpInteger::operator+(mpz_t rOperand)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_add(tempVal, mValue, rOperand);
    mpz_set(mValue, tempVal);

    return mValue;
}

mpz_t& GmpInteger::operator+=(mpz_t rOperand)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_add(tempVal, mValue, rOperand);
    mpz_set(mValue, tempVal);

    return mValue;
}

mpz_t& GmpInteger::operator+=(int rOperand)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_add_ui(tempVal, mValue, rOperand);
    mpz_set(mValue, tempVal);

    return mValue;
}

mpz_t& GmpInteger::operator-(mpz_t rOperand)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_sub(tempVal, mValue, rOperand);
    mpz_set(mValue, tempVal);

    return mValue;
}

mpz_t& GmpInteger::operator-=(mpz_t rOperand)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_sub(tempVal, mValue, rOperand);
    mpz_set(mValue, tempVal);

    return mValue;
}

GmpInteger& GmpInteger::add(const mpz_t value)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue + value)
    mpz_add(tempVal, mValue, value);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::sub(const mpz_t value)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue - value)
    mpz_sub(tempVal, mValue, value);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::sub(const std::uint64_t value)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue - value)
    mpz_sub_ui(tempVal, mValue, value);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::mod(const mpz_t modulVal)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue (mod modulVal))
    mpz_mod(tempVal, mValue, modulVal);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::mul(const mpz_t value)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue * value);
    mpz_mul(tempVal, mValue, value);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::div(const mpz_t value)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    // tempVal = (mValue * value);
    mpz_divexact(tempVal, mValue, value);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::pow(const std::uint32_t powValue)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_pow_ui(tempVal, mValue, powValue);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::powMod(const mpz_t exp, const mpz_t mod)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_powm(tempVal, mValue, exp, mod);
    mpz_set(mValue, tempVal);

    return *this;
}

GmpInteger& GmpInteger::powMod(const std::uint64_t exp, const mpz_t mod)
{
    mpz_t tempVal;
    mpz_init(tempVal);

    mpz_powm_ui(tempVal, mValue, exp, mod);
    mpz_set(mValue, tempVal);

    return *this;
}

void GmpInteger::powMod(mpz_t& result, const mpz_t base, const mpz_t exp, const mpz_t mod)
{
    mpz_powm(result, base, exp, mod);
}

const mpz_t& GmpInteger::getGmpValue()
{
    return mValue;
}

std::optional<int> GmpInteger::getStdValue()
{
    if (mValue) {
        return (*mValue->_mp_d);
    }

    return {};
}

}  // namespace kpi::cryptomech::lab1
