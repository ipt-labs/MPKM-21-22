#include "generators.h"
#include "tests.h"
#include "millerRabin.h"
#include "tripleDES.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale.h>
#include <sstream>

using namespace std;
using namespace System;
using namespace System::Security::Cryptography;

/*
// Возникает какой-то конфликт в коде...
#include <windows.h>
void print(string message, BigInteger el) // Вывод элементов
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Получаем дескриптор консоли
	SetConsoleTextAttribute(hStdOut, 7); // Меняем цвет текста на серый
	cout << message; // Выводим сообщение
	SetConsoleTextAttribute(hStdOut, 15); // Меняем цвет текста на белый
	Console::WriteLine(el); // Выводим элемент
}*/

// Для отображения прогренсса каждые frequancy итераций
#define show(finish, what) if (i % (finish / 100) == 0 || i == finish - 1) what

// Вывод прогресса в процентах
void outputProgress(ofstream & outputFile, string message, float progress);
// Вывод последовательностей байт
void outputSequences(ofstream & outputFile, byte * sequences);
// Вывод заголовка таблицы
void outputHeader(ofstream & outputFile, double a, double x21ae, double x21ai, double x21au);
// Вывод строки таблицы
void outputRow(ofstream & outputFile, string name, TestResult equiprobability, TestResult independence, TestResult uniformity);

// Вывод прогресса в процентах (шаблон)
template <typename Stream> void outputProgressTemplate(Stream& stream, string message, float progress);
// Вывод последовательностей байт (шаблон)
template <typename Stream> void outputSequencesTemplate(Stream& stream, byte* sequences);
// Вывод заголовка таблицы (шаблон)
template <typename Stream> void outputHeaderTemplate(Stream &stream, double a, double x21ae, double x21ai, double x21au);
// Вывод строки таблицы (шаблон)
template <typename Stream> void outputRowTemplate(Stream & stream, string name, TestResult equiprobability, TestResult independence, TestResult uniformity);

int main()
{
	setlocale(LC_ALL, "rus"); 

	const size_t sequencesSize = 1000000;  // Количество байт в последовательностях
	const size_t sequencesSizeTripleDES = 10000; // Количество байт в последовати TripleDES

	initRandomGenerators();  // Инициализация генераторов стартовыми значениями

	// Выделение памяти под последовательности
	byte* sequenceBBS = new byte[sequencesSize];
	byte* sequenceBBSByte = new byte[sequencesSize];
	byte* sequenceTripleDES = new byte[sequencesSizeTripleDES];

	// Файл вывода результата
	ofstream outputFile("./result.txt");

	ostringstream oss_for_sequenceBBS_768_as_str, oss_for_sequenceBBSByte_768_as_str, oss_for_sequenceBBS_1024_as_str, oss_for_sequenceBBSByte_1024_as_str;  // Потоки вывода

	// Заполнение массивов случайными байтами различных генераторов
	for (size_t i = 0; i < sequencesSize; i++)
	{
		sequenceBBS[i] = randBBS();
		if(i < 96) oss_for_sequenceBBS_768_as_str << sequenceBBS[i];
		if (i < 128) oss_for_sequenceBBS_1024_as_str << sequenceBBS[i];
		show(sequencesSize, outputProgress(outputFile, "Генерация последовательности генератором BBS: ", (i + 1.0f) / sequencesSize));
	}
	outputSequences(outputFile, sequenceBBS);
	string sequenceBBS_768_as_str(oss_for_sequenceBBS_768_as_str.str());
	string sequenceBBS_1024_as_str(oss_for_sequenceBBS_1024_as_str.str());

	for (size_t i = 0; i < sequencesSize; i++)
	{
		sequenceBBSByte[i] = randBBSByte();
		if (i < 96) oss_for_sequenceBBSByte_768_as_str << sequenceBBS[i];
		if (i < 128) oss_for_sequenceBBSByte_1024_as_str << sequenceBBS[i];
		show(sequencesSize, outputProgress(outputFile, "Генерация последовательности генератором BBS_bytes: ", (i + 1.0f) / sequencesSize));
	}
	outputSequences(outputFile, sequenceBBSByte);
	string sequenceBBSByte_768_as_str(oss_for_sequenceBBSByte_768_as_str.str());
	string sequenceBBSByte_1024_as_str(oss_for_sequenceBBSByte_1024_as_str.str());

	String^ sequenceBBS_768_as_str_msdn = gcnew String(sequenceBBS_768_as_str.c_str());
	String^ sequenceBBSByte_768_as_str_msdn = gcnew String(sequenceBBSByte_768_as_str.c_str());
	String^ sequenceBBS_1024_as_str_msdn = gcnew String(sequenceBBS_1024_as_str.c_str());
	String^ sequenceBBSByte_1024_as_str_msdn = gcnew String(sequenceBBSByte_1024_as_str.c_str());

	cout << "Генерируем последовательность, используя tripleDES" << endl;
	String^ inName = gcnew String("./inName.txt");
	String^ outName = gcnew String("./outName.txt");

	/*Console::WriteLine("Enter a password to produce a key:");
	cli::array<Byte>^ password = Encoding::Unicode->GetBytes(Console::ReadLine());
	cli::array<Byte>^ randomSalt = CreateRandomSalt(7); // salt = nonce

	Console::WriteLine("Creating a key with PasswordDeriveBytes...");
	// Create a PasswordDeriveBytes object and then create
	// a TripleDES key from the password and salt.
	PasswordDeriveBytes^ passwordDeriveBytes = gcnew PasswordDeriveBytes(password->ToString(), randomSalt);

	// Create a TripleDESCryptoServiceProvider object.
	TripleDESCryptoServiceProvider^ cryptoDESProvider = gcnew TripleDESCryptoServiceProvider();
	cli::array<Byte>^tdesKey = cryptoDESProvider->Key;
	cli::array<Byte>^tdesIV = cryptoDESProvider->IV;*/

	EncryptData(inName, outName);
	ifstream outNameFile("./outName.txt");  // Открытие outName.txt
	if (outNameFile.fail())  // Обработка ошибок
		cout << "Неудалось открыть файл!";
	else
	{
		char ch;
		size_t i = 0;
		while (outNameFile.get(ch))
		{
			sequenceTripleDES[i] = ch;
			cout << sequenceTripleDES[i];
			i++;
		}
	}

	ostringstream oss_for_TripleDES_768_as_str, oss_for_TripleDES_1024_as_str;
	for (size_t i = 0; i < 1024; i++)
	{
		if (i < 96) oss_for_TripleDES_768_as_str << sequenceTripleDES[i];
		if (i < 128) oss_for_TripleDES_1024_as_str << sequenceTripleDES[i];
	}
	string sequenceTripleDES_768_as_str(oss_for_TripleDES_768_as_str.str());
	string sequenceTripleDES_1024_as_str(oss_for_TripleDES_1024_as_str.str());
	String^ sequenceTripleDES_768_as_str_msdn = gcnew String(sequenceTripleDES_768_as_str.c_str());
	String^ sequenceTripleDES_1024_as_str_msdn = gcnew String(sequenceTripleDES_1024_as_str.c_str());

	BigInteger result_768_BBS, result_1024_BBS, result_768_BBS_Byte, result_1024_BBS_Byte, result_768_TripleDES, result_1024_TripleDES;
	result_768_BBS = BigInteger::Parse(sequenceBBS_768_as_str_msdn);
	result_768_BBS_Byte = BigInteger::Parse(sequenceBBSByte_768_as_str_msdn);
	result_1024_BBS = BigInteger::Parse(sequenceBBS_1024_as_str_msdn);
	result_1024_BBS_Byte = BigInteger::Parse(sequenceBBSByte_1024_as_str_msdn);
	result_768_TripleDES = BigInteger::Parse(sequenceTripleDES_768_as_str_msdn);
	result_1024_TripleDES = BigInteger::Parse(sequenceTripleDES_1024_as_str_msdn);

	auto max = 255;  // Максимальное значение байта
	auto r = 10u;  // Количество делений массива 

	auto le = max;  // l для равномерности
	auto li = pow(max, 2);  // l для независимости
	auto lu = max * (r - 1);  // l для однородности

	auto x21ae = sqrt(2.0 * le) * 2.3263479 + le;  // X^2 для равномерности 
	auto x21ai = sqrt(2.0 * li) * 2.3263479 + li;  // X^2 для независимости
	auto x21au = sqrt(2.0 * lu) * 2.3263479 + lu;  // X^2 для однородности

	// Тесты и вывод таблицы для a = 0.01
	outputHeader(outputFile, 0.01, x21ae, x21ai, x21au);
	outputRow(outputFile, "BBS", testEquiprobability(sequenceBBS, sequencesSize, le, x21ae),
		testIndependence(sequenceBBS, sequencesSize, li, x21ai), testUniformity(sequenceBBS, sequencesSize, lu, x21au));
	outputRow(outputFile, "BBS_bytes", testEquiprobability(sequenceBBSByte, sequencesSize, le, x21ae),
		testIndependence(sequenceBBSByte, sequencesSize, li, x21ai), testUniformity(sequenceBBSByte, sequencesSize, lu, x21au));
	outputRow(outputFile, "TripleDES", testEquiprobability(sequenceTripleDES, sequencesSizeTripleDES, le, x21ae),
		testIndependence(sequenceTripleDES, sequencesSizeTripleDES, li, x21ai), testUniformity(sequenceTripleDES, sequencesSizeTripleDES, lu, x21au));

	x21ae = sqrt(2.0 * le) * 1.6448536 + le;
	x21ai = sqrt(2.0 * li) * 1.6448536 + li;
	x21au = sqrt(2.0 * lu) * 1.6448536 + lu;

	// Тесты и вывод таблицы для a = 0.05
	outputHeader(outputFile, 0.05, x21ae, x21ai, x21au);
	outputRow(outputFile, "BBS", testEquiprobability(sequenceBBS, sequencesSize, le, x21ae),
		testIndependence(sequenceBBS, sequencesSize, li, x21ai), testUniformity(sequenceBBS, sequencesSize, lu, x21au));
	outputRow(outputFile, "BBS_bytes", testEquiprobability(sequenceBBSByte, sequencesSize, le, x21ae),
		testIndependence(sequenceBBSByte, sequencesSize, li, x21ai), testUniformity(sequenceBBSByte, sequencesSize, lu, x21au));
	outputRow(outputFile, "TripleDES", testEquiprobability(sequenceTripleDES, sequencesSizeTripleDES, le, x21ae),
		testIndependence(sequenceTripleDES, sequencesSizeTripleDES, li, x21ai), testUniformity(sequenceTripleDES, sequencesSizeTripleDES, lu, x21au));
		
	cout << "Результат теста Миллера-Рабина для result_768_BBS: "; 
	if (millerRabin(result_768_BBS, 768) == true) cout << "простое" << endl;
	else cout << "составное" << endl;
	cout << "Результат теста Миллера-Рабина для result_768_BBS_Byte: ";
	if (millerRabin(result_768_BBS_Byte, 768) == true) cout << "простое" << endl;
	else cout << "составное" << endl;
	cout << "Результат теста Миллера-Рабина для result_768_TripleDES: ";
	if (millerRabin(result_768_TripleDES, 768) == true) cout << "простое" << endl;
	else cout << "составное" << endl;

	cout << "Результат теста Миллера-Рабина для result_1024_BBS: ";
	if (millerRabin(result_1024_BBS, 1024) == true) cout << "простое" << endl;
	else cout << "составное" << endl;
	cout << "Результат теста Миллера-Рабина для result_1024_BBS_Byte: ";
	if (millerRabin(result_1024_BBS_Byte, 1024) == true) cout << "простое" << endl;
	else cout << "составное" << endl;
	cout << "Результат теста Миллера-Рабина для result_1024_TripleDES: ";
	if (millerRabin(result_1024_TripleDES, 1024) == true) cout << "простое" << endl;
	else cout << "составное" << endl;

	cout << "Результат теста Миллера-Рабина для 41: ";
	if (millerRabin(BigInteger::Parse("41"), 1024) == true) cout << "простое" << endl;
	else cout << "составное" << endl;
	

	// Закрытие файла результата
	outNameFile.close();
	// Закрытие файла результата
	outputFile.close();

	// Освобождение памяти
	delete[] sequenceBBS;
	delete[] sequenceBBSByte;

	cin.get();
	cin.get();
	return 0;
}

void outputProgress(ofstream& outputFile, string message, float progress)
{
	cout << "\r";
	outputProgressTemplate(cout, message, progress);
	if (progress == 1.0f)
		outputProgressTemplate(outputFile, message, progress);
}

void outputSequences(ofstream &outputFile, byte* sequences)
{
	outputSequencesTemplate(cout, sequences);
	outputSequencesTemplate(outputFile, sequences);
}

void outputHeader(ofstream& outputFile, double a, double x21ae, double x21ai, double x21au)
{
	outputHeaderTemplate(cout, a, x21ae, x21ai, x21au);
	outputHeaderTemplate(outputFile, a, x21ae, x21ai, x21au);
}

void outputRow(ofstream& outputFile, string name, TestResult equiprobability, TestResult independence, TestResult uniformity)
{
	outputRowTemplate(cout, name, equiprobability, independence, uniformity);
	outputRowTemplate(outputFile, name, equiprobability, independence, uniformity);
}

template <typename Stream> void outputProgressTemplate(Stream& stream, string message, float progress)
{
	stream << message << fixed << setprecision(0) << progress * 100.0f << "%" << (progress == 1.0f ? "\n" : "");
}

template <typename Stream> void outputSequencesTemplate(Stream& stream, byte *sequences)
{
	stream << "Вывод первых 128 байт:\n";
	for (int i = 0; i < 128; i++)
		stream << sequences[i] << (i == 127 ? "\n\n" : " ");
}

template <typename Stream> void outputRowTemplate(Stream& stream, string name, TestResult equiprobability, TestResult independence, TestResult uniformity)
{
	stringstream col1, col2, col3;
	col1 << " | " << fixed << setprecision(3) << equiprobability.x2 << (equiprobability.right ? " <= Хи^2 => +" : " > Хи^2 => -");
	col2 << " | " << fixed << setprecision(3) << independence.x2 << (independence.right ? " <= Хи^2 => +" : " > Хи^2 => -");
	col3 << " | " << fixed << setprecision(3) << uniformity.x2 << (uniformity.right ? " <= Хи^2 => +" : " > Хи^2 => -");
	stream << "| " << setw(12) << left << name
		<< setw(31) << col1.str() << setw(31) << col2.str() << setw(31) << col3.str() << " |" << endl <<
		"-------------------------------------------------------------------------------------------------------------" << endl;
}

template <typename Stream> void outputHeaderTemplate(Stream &stream, double a, double x21ae, double x21ai, double x21au)
{
	stringstream headerText;
	headerText << "Aльфа = " << fixed << setprecision(2) << a << " | => Хи^2 = "
		<< setw(19) << left << fixed << setprecision(3) << x21ae << "| => Хи^2 = "
		<< setw(19) << left << fixed << setprecision(3) << x21ai << "| => Хи^2 = "
		<< setw(19) << left << fixed << setprecision(3) << x21au << "";
	stream << endl;
	stream << "-------------------------------------------------------------------------------------------------------------" << endl;
	stream << "| Результат: '+' = 'прошел тест', '-' = 'не прошел тест'                                                    |" << endl;
	stream << "-------------------------------------------------------------------------------------------------------------" << endl;
	stream << "| " << setw(43) << left << headerText.str() << "|" << endl;
	stream << "-------------------------------------------------------------------------------------------------------------" << endl;
	stream << "| Генератор    | Равновероятность             | Независимость                | Однородность                 |" << endl;
	stream << "-------------------------------------------------------------------------------------------------------------" << endl;
}
