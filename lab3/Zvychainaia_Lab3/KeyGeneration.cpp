#pragma once

#include "KeyGeneration.h"
#include "RandomBBS.h"
#include "MillerRabin.h"

#include <iostream>

void generate_key(unsigned char* key, size_t lenght)  // Генерация симметричного ключа
{
	size_t byte_lenght = lenght / 8;  // Количество байт
	Integer p, q;  // Переменные
	auto bytes = new byte[byte_lenght];  // Байты ключа
	do
	{			
		do
		{
			for (size_t i = 0; i < byte_lenght; i++)  // Проход по массиву байт
				bytes[i] = rand_bbs();  // Случайный байт
			p = Integer(bytes, byte_lenght);  // Создание большого числа из массива байт
		} while (!miller_rabin(p));  // Проверка p алогритмом Миллера-Рабина
		q = 2 * p + 1;  // Вычисление q = 2p + 1
	} while (!miller_rabin(q));  // Проверка q алогритмом Миллера-Рабина
	for (int i = (int)byte_lenght - 1; i >= 0; i--)  // Проход по байтам q
		key[i] = q.GetByte(byte_lenght - i - 1);  // Копирование байта в ключ
	delete[] bytes;  // Освобождение памяти
}
