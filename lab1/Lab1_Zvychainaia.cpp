#include <iostream>
#include <string>
#include <chrono>
#include <windows.h>
#include <map>

using namespace std;
using namespace std::chrono;
using namespace System::Globalization; // Для NumberStyles::AllowHexSpecifier
using namespace System::Numerics;
using namespace System;

void input(string message, string el)  // Ввод элементов
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  // Получаем дескриптор консоли
	SetConsoleTextAttribute(hStdOut, 7);  // Меняем цвет текста на серый
	cout << message;  // Выводим сообщение
	SetConsoleTextAttribute(hStdOut, 15);  // Меняем цвет текста на белый
	cin >> el; // Вводим элемент
}

void print(string message, BigInteger el)  // Вывод элементов
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  // Получаем дескриптор консоли
	SetConsoleTextAttribute(hStdOut, 7);  // Меняем цвет текста на серый
	cout << message;  // Выводим сообщение
	SetConsoleTextAttribute(hStdOut, 15);  // Меняем цвет текста на белый
	Console::WriteLine(el);  // Выводим элемент
}

void print(string message, string el)  // Вывод элементов
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  // Получаем дескриптор консоли
	SetConsoleTextAttribute(hStdOut, 7);  // Меняем цвет текста на серый
	cout << message;  // Выводим сообщение
	SetConsoleTextAttribute(hStdOut, 15);  // Меняем цвет текста на белый
	cout << el << endl;  // Выводим элемент
}
string elapsedTime(time_point<system_clock> start, time_point<system_clock> end)  // Конвертирование время в строку
{
	int count = duration_cast<microseconds>(end - start).count();  // Считаем прошедшее время
	return count > 1000 ? to_string(count / 1000) + " мс." : to_string(count) + " мкс.";  // Возвращаем строку
}

long elapsedTime2(time_point<system_clock> start, time_point<system_clock> end)  // Конвертирование время в строку
{
	return duration_cast<microseconds>(end - start).count();  // Возвращаем прошедшее время в мкс
}

static BigInteger mod(BigInteger a, BigInteger b)
{
	if (a > BigInteger(0))  // Если делимое больше 0
		return a % b;  // Возвращаем модуль a на b
	if ((a % b) != BigInteger(0))  // Если модуль не равен 0
		// Возвращаем остаток от деления
		return BigInteger::Subtract(BigInteger::Multiply(BigInteger::Add(BigInteger::Divide(BigInteger::Abs(a),
			b), BigInteger(1)), b), BigInteger::Abs(a));
	return 0;  // Возвращаем 0
}

void print(BigInteger a, BigInteger b, BigInteger c, BigInteger mod_amount) {
	BigInteger result;
	time_point<system_clock> start, end;  // Переменные для измерения времени

	print("Первый элемент (a): ", a);
	print("Второй элемент (b): ", b);
	print("Третий элемент (c): ", c);
	print("Модуль (mod): ", mod_amount);
	cout << endl;

	start = system_clock::now();
	result = mod(BigInteger(0), BigInteger(1));
	end = system_clock::now();

	start = system_clock::now();
	c = mod(c, mod_amount);
	end = system_clock::now();

	print("Третий элемент по модулю:", c);
	print("Время работы операции взятия по модулю: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = mod(BigInteger::Add(c, a), mod_amount);
	end = system_clock::now();

	print("Сумма первого и третьего по модулю: ", result);
	print("Время работы операции сложения: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = mod(BigInteger::Subtract(a, b), mod_amount);
	end = system_clock::now();

	print("Разница первого элемента и второго по модулю: ", result);
	print("Время работы операции разница: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = mod(BigInteger::Multiply(a, b), mod_amount);
	end = system_clock::now();

	print("Умножение первого элемента на второй по модулю: ", result);
	print("Время работы операции умножения: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = mod(BigInteger::Pow(a, 2), mod_amount);
	end = system_clock::now();

	print("Возведение первого элемента в квадрат по модулю (моя функция): ", result);
	print("Время работы операции возведения в квадрат первого элемента: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = BigInteger::ModPow(a, BigInteger(2), mod_amount);
	end = system_clock::now();

	print("Возведение первого элемента в квадрат по модулю (встроенная функция): ", result);
	print("Время работы операции возведения в квадрат первого элемента: ", elapsedTime(start, end));
	cout << endl;

	start = system_clock::now();
	result = BigInteger::Divide(a, b);
	end = system_clock::now();

	print("Целая часть от деления a на b: ", result);
	print("Время работы операции деление: ", elapsedTime(start, end));
	cout << endl;

	print("Тест на корректность (проверка дистрибутивности): ", "");
	print("Наш первый элемент (a): ", a);
	print("Наш второй элемент (b): ", b);
	print("Наш третий элемент (c): ", BigInteger::Subtract(c, BigInteger(1)));
	print("Результат (a+b)*c : ", mod(BigInteger::Multiply(BigInteger::Add(a, b), BigInteger::Subtract(c, BigInteger(1))), mod_amount));
	print("Результат b*c+c*a : ", mod(BigInteger::Add(BigInteger::Multiply(b, BigInteger::Subtract(c, BigInteger(1))), BigInteger::Multiply(BigInteger::Subtract(c, BigInteger(1)), a)), mod_amount));
	cout << endl;
}

void average_time_count(BigInteger a, BigInteger b, BigInteger mod_amount, BigInteger iter_amount) {
	BigInteger num_iter = BigInteger(0);
	BigInteger result;
	BigInteger average_time_count_add = BigInteger(0);
	BigInteger average_time_count_sub = BigInteger(0);
	BigInteger average_time_count_mul = BigInteger(0);
	BigInteger average_time_count_div = BigInteger(0);
	BigInteger average_time_count_powb = BigInteger(0);
	BigInteger average_time_count_pow2 = BigInteger(0);
	BigInteger average_time_count_pow2_rem = BigInteger(0);
	//---------------------------------------------------
	BigInteger average_time_count_add_my = BigInteger(0);
	BigInteger average_time_count_sub_my = BigInteger(0);
	BigInteger average_time_count_mul_my = BigInteger(0);
	BigInteger average_time_count_div_my = BigInteger(0);
	BigInteger average_time_count_pow2_my = BigInteger(0);
	time_point<system_clock> start, end;  // Переменные для измерения времени
	start = system_clock::now();
	a = mod(a, mod_amount);
	end = system_clock::now();

	while (BigInteger::Compare(num_iter, iter_amount) != 0) { // Пока не пройдет iter_amount итераций, делаем

		start = system_clock::now();
		result = BigInteger::Remainder(BigInteger::Add(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_add = BigInteger::Add(average_time_count_add, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::Remainder(BigInteger::Subtract(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_sub = BigInteger::Add(average_time_count_sub, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::Remainder(BigInteger::Multiply(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_mul = BigInteger::Add(average_time_count_mul, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::Remainder(BigInteger::Divide(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_div = BigInteger::Add(average_time_count_div, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::ModPow(a, 2, mod_amount);
		end = system_clock::now();
		average_time_count_pow2 = BigInteger::Add(average_time_count_pow2, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::Remainder(BigInteger::Pow(a, 2), mod_amount);
		end = system_clock::now();
		average_time_count_pow2_rem = BigInteger::Add(average_time_count_pow2_rem, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = BigInteger::ModPow(a, b, mod_amount);
		end = system_clock::now();
		average_time_count_powb = BigInteger::Add(average_time_count_powb, BigInteger(elapsedTime2(start, end)));

		// -------------With my func "mod":
		start = system_clock::now();
		result = mod(BigInteger::Add(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_add_my = BigInteger::Add(average_time_count_add_my, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = mod(BigInteger::Subtract(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_sub_my = BigInteger::Add(average_time_count_sub_my, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = mod(BigInteger::Multiply(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_mul_my = BigInteger::Add(average_time_count_mul_my, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = mod(BigInteger::Divide(a, b), mod_amount);
		end = system_clock::now();
		average_time_count_div_my = BigInteger::Add(average_time_count_div_my, BigInteger(elapsedTime2(start, end)));

		start = system_clock::now();
		result = mod(BigInteger::Pow(a, 2), mod_amount);
		end = system_clock::now();
		average_time_count_pow2_my = BigInteger::Add(average_time_count_pow2_my, BigInteger(elapsedTime2(start, end)));

		//a = BigInteger::Add(a, BigInteger(1));
		//b = BigInteger::Add(b, BigInteger(1));
		num_iter++;
	}
	//average_time_count_add = BigInteger::Divide(average_time_count_add, BigInteger(iter_amount));
	//average_time_count_sub = BigInteger::Divide(average_time_count_sub, BigInteger(iter_amount));
	//average_time_count_mul = BigInteger::Divide(average_time_count_mul, BigInteger(iter_amount));
	//average_time_count_div = BigInteger::Divide(average_time_count_div, BigInteger(iter_amount));
	if (average_time_count_add > BigInteger(1000)) 
		print("Время работы операции Add (в мс): ", BigInteger::Divide(average_time_count_add, BigInteger(1000)));
	else print("Время работы операции Add (в мкс): ", average_time_count_add);

	if (average_time_count_add_my > BigInteger(1000))
		print("Время работы операции Add (my mod_func, в мс): ", BigInteger::Divide(average_time_count_add_my, BigInteger(1000)));
	else print("Время работы операции Add (my mod_func, в мкс): ", average_time_count_add_my);

	if (average_time_count_sub > BigInteger(1000))
		print("Время работы операции Subtract (в мс): ", BigInteger::Divide(average_time_count_sub, BigInteger(1000)));
	else print("Время работы операции Subtract (в мкс): ", average_time_count_sub);

	if (average_time_count_sub_my > BigInteger(1000))
		print("Время работы операции Subtract (my mod_func, в мс): ", BigInteger::Divide(average_time_count_sub_my, BigInteger(1000)));
	else print("Время работы операции Subtract (my mod_func, в мкс): ", average_time_count_sub_my);
	//--------------------Multiply--------------------------------------
	if (average_time_count_mul > BigInteger(1000000))
		print("Время работы операции Multiply (в с): ", BigInteger::Divide(average_time_count_mul, BigInteger(1000000)));
	else 
		if (average_time_count_mul > BigInteger(1000))
			print("Время работы операции Multiply (в мс): ", BigInteger::Divide(average_time_count_mul, BigInteger(1000)));
		else print("Время работы операции Multiply (в мкс): ", average_time_count_mul);
	//---------------------Multiply(My mod_func)------------------------
	if (average_time_count_mul_my > BigInteger(1000000))
		print("Время работы операции Multiply (my mod_func, в с): ", BigInteger::Divide(average_time_count_mul_my, BigInteger(1000000)));
	else
		if (average_time_count_mul_my > BigInteger(1000))
			print("Время работы операции Multiply (my mod_func, в мс): ", BigInteger::Divide(average_time_count_mul_my, BigInteger(1000)));
		else print("Время работы операции Multiply (my mod_func, в мкс): ", average_time_count_mul_my);
	//-------------------------------------------------------------------
	if (average_time_count_div > BigInteger(1000))
		print("Время работы операции Divide (в мс): ", BigInteger::Divide(average_time_count_div, BigInteger(1000)));
	else print("Время работы операции Divide (в мкс): ", average_time_count_div);

	if (average_time_count_div_my > BigInteger(1000))
		print("Время работы операции Divide (my mod_func, в мс): ", BigInteger::Divide(average_time_count_div_my, BigInteger(1000)));
	else print("Время работы операции Divide (my mod_func, в мкс): ", average_time_count_div_my);
	//---------------------------ModPow----------------------------------------
	if (average_time_count_pow2 > BigInteger(1000000))
		print("Время работы операции ModPow (a^2, в с): ", BigInteger::Divide(average_time_count_pow2, BigInteger(1000000)));
	else 
		if (average_time_count_pow2 > BigInteger(1000))
			print("Время работы операции ModPow (a^2, в мс): ", BigInteger::Divide(average_time_count_pow2, BigInteger(1000)));
		else print("Время работы операции ModPow (a^2, в мкс): ", average_time_count_pow2);
	//----------------------------Pow(Rem_func)------------------------------------
	if (average_time_count_pow2_rem > BigInteger(1000000))
		print("Время работы операции Pow (a^2, Rem_func, в с): ", BigInteger::Divide(average_time_count_pow2_rem, BigInteger(1000000)));
	else
		if (average_time_count_pow2_rem > BigInteger(1000))
			print("Время работы операции Pow (a^2, Rem_func, в мс): ", BigInteger::Divide(average_time_count_pow2_rem, BigInteger(1000)));
		else print("Время работы операции Pow (a^2, Rem_func, в мкс): ", average_time_count_pow2_rem);
	//----------------------------Pow(my mod_func)------------------------------------
	if (average_time_count_pow2_my > BigInteger(1000000))
		print("Время работы операции Pow (a^2, my mod_func, в с): ", BigInteger::Divide(average_time_count_pow2_my, BigInteger(1000000)));
	else 
		if (average_time_count_pow2_my > BigInteger(1000))
			print("Время работы операции Pow (a^2, my mod_func, в мс): ", BigInteger::Divide(average_time_count_pow2_my, BigInteger(1000)));
		else print("Время работы операции Pow (a^2, my mod_func, в мкс): ", average_time_count_pow2_my);
	//----------------------------ModPow (a^b)-----------------------------------------
	if (average_time_count_powb > BigInteger(1000000))
		print("Время работы операции ModPow (a^b, в с): ", BigInteger::Divide(average_time_count_powb, BigInteger(1000000)));
	else 
		if (average_time_count_powb > BigInteger(1000))
			print("Время работы операции ModPow (a^b, в мс): ", BigInteger::Divide(average_time_count_powb, BigInteger(1000)));
		else print("Время работы операции ModPow (a^b, в мкс): ", average_time_count_powb);
}

void main()
{
	setlocale(LC_ALL, "rus");  // Поддержка кириллицы
	// Переменные (seed)
	BigInteger a_256 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 256), BigInteger(50000000000));
	BigInteger b_256 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 256), BigInteger(10000000000));
	BigInteger c_256 = BigInteger::Add(BigInteger::Pow(BigInteger(2), 256), BigInteger(500));

	BigInteger a_512 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 512), BigInteger(50000000000));
	BigInteger b_512 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 512), BigInteger(10000000000));
	BigInteger c_512 = BigInteger::Add(BigInteger::Pow(BigInteger(2), 512), BigInteger(500));

	BigInteger a_1024 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 1024), BigInteger(50000000000));
	BigInteger b_1024 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 1024), BigInteger(10000000000));
	BigInteger c_1024 = BigInteger::Add(BigInteger::Pow(BigInteger(2), 1024), BigInteger(500));

	BigInteger a_2048 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 2048), BigInteger(50000000000));
	BigInteger b_2048 = BigInteger::Subtract(BigInteger::Pow(BigInteger(2), 2048), BigInteger(10000000000));
	BigInteger c_2048 = BigInteger::Add(BigInteger::Pow(BigInteger(2), 2048), BigInteger(500));

	// Модули
	BigInteger mod_256 = BigInteger::Pow(BigInteger(2), 256);
	BigInteger mod_512 = BigInteger::Pow(BigInteger(2), 512);
	BigInteger mod_1024 = BigInteger::Pow(BigInteger(2), 1024);
	BigInteger mod_2048 = BigInteger::Pow(BigInteger(2), 2048);

	/*print(a_256, b_256, c_256, mod_256);
	cout << endl;
	print(a_512, b_512, c_512, mod_512);
	cout << endl;
	print(a_1024, b_1024, c_1024, mod_1024);
	cout << endl;
	print(a_2048, b_2048, c_2048, mod_2048);*/
	int sizeIter_amount = 6; // 1 000,   10 000,   100 000,   1 000 000,   10 000 000,   1 000 000 000
	int *Iter_amount = new int[sizeIter_amount];
	for (int i = 0, j = 1000; i < sizeIter_amount; i++) {
		Iter_amount[i] = j;
		j *= 10;
	}
	Iter_amount[5] = 1000000000; // 1 000 000 000
	cout << "-------------------------------------------------------" << endl;
	/*for (int i = 0; i < sizeIter_amount; i++) {
		cout << "Замеры среднего времени базовых операций по модулю 2^256 для " << Iter_amount[i] << " итераций: " << endl;
		average_time_count(a_256, b_256, mod_256, BigInteger(Iter_amount[i]));
		cout << endl << "Замеры среднего времени базовых операций по модулю 2^512 для " << Iter_amount[i] << " итераций: " << endl;
		average_time_count(a_512, b_512, mod_512, BigInteger(Iter_amount[i]));
		cout << endl << "Замеры среднего времени базовых операций по модулю 2^1024 для " << Iter_amount[i] << " итераций: " << endl;
		average_time_count(a_1024, b_1024, mod_1024, BigInteger(Iter_amount[i]));
		cout << endl << "Замеры среднего времени базовых операций по модулю 2^2048 для " << Iter_amount[i] << " итераций: " << endl;
		average_time_count(a_2048, b_2048, mod_2048, BigInteger(Iter_amount[i]));
		cout << "-------------------------------------------------------" << endl;
	}*/
	cout << "Замеры среднего времени базовых операций по модулю 2^256 для " << Iter_amount[5] << " итераций: " << endl;
	average_time_count(a_256, b_256, mod_256, BigInteger(Iter_amount[5]));
	cout << endl << "Замеры среднего времени базовых операций по модулю 2^512 для " << Iter_amount[5] << " итераций: " << endl;
	average_time_count(a_512, b_512, mod_512, BigInteger(Iter_amount[5]));
	cout << endl << "Замеры среднего времени базовых операций по модулю 2^1024 для " << Iter_amount[5] << " итераций: " << endl;
	average_time_count(a_1024, b_1024, mod_1024, BigInteger(Iter_amount[5]));
	cout << endl << "Замеры среднего времени базовых операций по модулю 2^2048 для " << Iter_amount[5] << " итераций: " << endl;
	average_time_count(a_2048, b_2048, mod_2048, BigInteger(Iter_amount[5]));
	cout << "-------------------------------------------------------" << endl;
	delete[] Iter_amount;

	/*BigInteger a_bigInt = BigInteger::Parse("17BC9E6E5CA3078D067DDB15CB2DF7B7D5E6437F99FE5FBB91750C3D2E4AFC7FCABC5F8795D0570EBF7A90AA0603D88FB10169B55D592959BBF88E2F141B16C3", NumberStyles::AllowHexSpecifier);
	BigInteger b_bigInt = BigInteger::Parse("1F2A11C94AB245C494806E38BDADD88C3EBFD5E688A51FDDFDFFE23069EFF0BAE0BC82B1B2D07E6B56764F01B331B47D7DDE8641778784A6AEF78BC72F337B3E", NumberStyles::AllowHexSpecifier);
	BigInteger mod_bigInt = BigInteger::Parse("10C6E1E34644B70666262C3DC6BD321A8C8A65753C6D90F4628EB5866FA0B4D494CC9294BF4A7F252B74521DFBC9D290AFB73B7844D896C658F5A76423D074D8", NumberStyles::AllowHexSpecifier);
	print("A: ", a_bigInt);
	print("B: ", b_bigInt);
	print("N: ", mod_bigInt);
	print("A+B: ", BigInteger::Add(a_bigInt, b_bigInt));
	print("(A+B)modN: ", mod(BigInteger::Add(a_bigInt, b_bigInt), mod_bigInt));
	print("A-B: ", BigInteger::Subtract(a_bigInt, b_bigInt));
	print("(A-B)modN: ", mod(BigInteger::Subtract(a_bigInt, b_bigInt), mod_bigInt));
	print("A*B: ", BigInteger::Multiply(a_bigInt, b_bigInt));
	print("(A*B)modN: ", mod(BigInteger::Multiply(a_bigInt, b_bigInt), mod_bigInt));
	if (BigInteger::Compare(a_bigInt, b_bigInt) == 1) {
		print("A/B: ", BigInteger::Divide(a_bigInt, b_bigInt));
		print("(A/B)modN: ", mod(BigInteger::Divide(a_bigInt, b_bigInt), mod_bigInt));
		print("Rem_(A/B)modN: ", mod(BigInteger::Remainder(a_bigInt, b_bigInt), mod_bigInt));
	}
	else {
		print("B/A: ", BigInteger::Divide(b_bigInt, a_bigInt));
		print("(B/A)modN: ", mod(BigInteger::Divide(b_bigInt, a_bigInt), mod_bigInt));
		print("Rem_(B/A)modN: ", mod(BigInteger::Remainder(b_bigInt, a_bigInt), mod_bigInt));
	}
	cout << "-------------------------------------------------------" << endl;*/



	cin.get();
	cin.get();
}
