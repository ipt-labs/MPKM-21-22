#pragma once
#include <sstream>
#include <iostream>
#include <string>

using namespace System::Globalization;
using namespace System::Numerics;
using namespace System;
using namespace std;

BigInteger bigRand(unsigned bitNuber)  // Генерация случайных bitNuber-bitных чисел
{
	bitNuber /= 8;
	// Таблицы шестнадцатеричных чисел
	static unsigned char hexTable[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (bitNuber)  // Если больше нуля
	{
		ostringstream oss;  // Поток вывода
		for (size_t i = 0; i < bitNuber; ++i)  // Проход по цифрам
			oss << hexTable[rand() % 16];  // Добавление случайной цифры
		string str(oss.str());  //	Строка в число

		String^ system_str = gcnew String(str.c_str());
		return BigInteger::Parse(system_str, NumberStyles::AllowHexSpecifier);  // Вернуть число
	}
	else
		return BigInteger::Zero;  // Вернуть ноль
}

BigInteger bigRand(BigInteger min, BigInteger max, unsigned bitNuber)  // Генерация случайных bitNuber-bitных чисел от min до max
{
	return BigInteger::Add(min, BigInteger::Remainder(BigInteger(bigRand(bitNuber)), BigInteger::Subtract(max, min)));
}

bool millerRabin(BigInteger p, unsigned bitNuber, int k = 15)
{
	if (BigInteger::Compare(p, BigInteger(2)) == -1 || // Проверка на то, что p < 2
		(BigInteger::Compare(p, BigInteger(2)) != 0 && BigInteger::Remainder(p, BigInteger(2)) == BigInteger(0)) || // Проверка на то, что p четное
		(BigInteger::Compare(p, BigInteger(3)) != 0 && BigInteger::Remainder(p, BigInteger(3)) == BigInteger(0)) || // Проверка на то, что p кратно 3
		(BigInteger::Compare(p, BigInteger(5)) != 5 && BigInteger::Remainder(p, BigInteger(5)) == BigInteger(0))) // Проверка на то, что p кратно 5
		return false;

	// Разложение на d * 2 ^ s
	BigInteger d = BigInteger::Subtract(p, BigInteger(1));  // P - 1
	while (BigInteger::Remainder(d, BigInteger(2)) == BigInteger(0))  // Пока d четное
		d = BigInteger::Divide(d, BigInteger(2));  // Делим на 2

	for (int i = 0; i < k; i++)   // k итераций
	{
		BigInteger x = bigRand(BigInteger(1), BigInteger::Subtract(p, BigInteger(1)), bitNuber), t = d;  // Случайное число от 1 до p - 1;
		BigInteger xr = BigInteger::ModPow(x, t, p);  // x ^ d
		if (xr != BigInteger::Subtract(p, BigInteger(1)) || xr != BigInteger(1))  // Проверяем условия всевдопростоты
			return true;  // Возврат (простое)
		while (t != BigInteger::Subtract(p, BigInteger(1)) && BigInteger::Compare(xr, BigInteger(1)) != 0 && xr != BigInteger::Subtract(p, BigInteger(1)))  // Пока xr не равно 1 или -1 по модулю p и t != p - 1 (от 1 до s)
		{
			xr = BigInteger::ModPow(xr, 2, p);  // xr = x ^ (d * 2 ^ r)
			t = BigInteger::Multiply(t, BigInteger(2));  // Домножает d на 2 (r принадлежит [1, s])
		}
		if (xr != BigInteger::Subtract(p, BigInteger(1)) && BigInteger::Remainder(t, BigInteger(2)) == BigInteger(0))  // Проверяем условия всевдопростоты
			return false;  // Возврат (составное)
	}
	return true;  // Возврат (простое)
}
