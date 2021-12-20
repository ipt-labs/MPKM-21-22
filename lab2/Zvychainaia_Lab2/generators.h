#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace System::Globalization;
using namespace System::Numerics;

struct byte  // Структура для удобства (по суте char но выводится как int)
{
	unsigned char value;

	byte()
	{
		this->value = 0;
	}

	byte(const int& value)
	{
		this->value = (unsigned char)value;
	}

	operator unsigned int()
	{
		return (unsigned int)value;
	}

	byte& operator =(const unsigned int &value)
	{
		this->value = (unsigned char)value;
		return *this;
	}

	byte& operator =(const int& value)
	{
		this->value = (unsigned char)value;
		return *this;
	}
};

// Начальные значения для функций рандома
unsigned int seedBBS;
unsigned int seedBBSByte;

static ref struct BBSSeed
{
	static BigInteger n;
	static BigInteger r;
};

// Инициализация начальных значений
void initRandomGenerators()
{
	seedBBS = 54287434u;
	seedBBSByte = 2347532544u;
	BBSSeed::n = BigInteger::Parse("D5BBB96D30086EC484EBA3D7F9CAEB07", NumberStyles::AllowHexSpecifier) * 
		BigInteger::Parse("425D2B9BFDB25B9CF6C416CC6E37B59C1F", NumberStyles::AllowHexSpecifier);
	BBSSeed::r = BigInteger::Parse("675215CC3E227D321097E1DB049F1", NumberStyles::AllowHexSpecifier);
}

byte randBBS()
{
	for (int i = 0; i < 8; i++)
	{
		BBSSeed::r = BigInteger::ModPow(BBSSeed::r, 2, BBSSeed::n);
		seedBBS = static_cast<int>(BBSSeed::r % 2) ? seedBBS << 1 | 1u : seedBBS << 1 & ~1u;
	}
	return static_cast<byte>(seedBBS);
}

byte randBBSByte()
{
	BBSSeed::r = BigInteger::ModPow(BBSSeed::r, 2, BBSSeed::n);
	seedBBSByte = static_cast<unsigned int>(BBSSeed::r % 256);
	return static_cast<byte>(seedBBSByte);
}


