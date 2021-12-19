#pragma once

#include <cryptopp/integer.h>

using namespace CryptoPP;

Integer modulo(Integer base, Integer exponent, Integer mod);  // Степень по модулю
unsigned char rand_bbs();  // Случайный байт по алгоритму BBS
