#include "KeyGeneration.h"
#include "MillerRabin.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cryptopp/elgamal.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nr.h>
#include <openssl/aes.h>

using namespace std;
using namespace CryptoPP;

SecByteBlock key_to_bytes(ElGamalKeys::PrivateKey& key)  // Преобразование приватного ключа Эль-Гамаля в байты
{
	Integer exponent = key.GetPrivateExponent();  // Получение приватного элемента
	unsigned byteCount = exponent.ByteCount();  // Получение количества байт
	SecByteBlock buffer(byteCount);  // Байты приватного ключа 
	for (size_t i = 0; i < byteCount; i++)  // Проход по ключу
		buffer[i] = exponent.GetByte(byteCount - i - 1);  // Получение байта
	return buffer;  // Возврат байт
}

SecByteBlock key_to_bytes(ElGamalKeys::PublicKey& key)  // Преобразование публичного ключа Эль-Гамаля в байты
{
	Integer exponent = key.GetPublicElement();  // Получение публичного элемента
	unsigned byteCount = exponent.ByteCount();  // Получение количества байт
	SecByteBlock buffer(byteCount);  // Байты публичного ключа 
	for (size_t i = 0; i < byteCount; i++)  // Проход по ключу
		buffer[i] = exponent.GetByte(byteCount - i - 1);  // Получение байта
	return buffer;  // Возврат байт
}

std::string to_string(byte* key, size_t lenght)  // Преобразование массива байт в hex строку
{
	std::stringstream stream;  // Поток
	for (size_t i = 0; i < lenght; i++)  // Проход по байтам
		stream << std::hex << std::setw(2) << std::setfill('0') <<  // Вывод байта в поток
		std::uppercase << (unsigned int)key[i];
	return stream.str();  // Преобразование в строку
}

string to_string(SecByteBlock bytes)  // Вывод массива байт в виде hex числа
{
	return to_string(bytes, bytes.size());  // Возврат hex числа из массива
}

typedef NR<SHA256>::Signer ElGamalSigner;  // Класс модифицированной подписи Эль-Гамаля 
typedef NR<SHA256>::Verifier ElGamalVerifier;  // Класс проверки подписи Эль-Гамаля 

constexpr int ELGAMAL_KEY_BIT_LENGTH = 256;  // Длина ключей Эль-Гамаля (в битах)
constexpr int AES_KEY_LENGTH = AES_BLOCK_SIZE;  // Длина симметричного ключа (в байтах)
constexpr int ELGAMAL_KEY_LENGTH = ELGAMAL_KEY_BIT_LENGTH / 8;  // Длина ключей Эль-Гамаля (в байтах)
constexpr int AES_KEY_BIT_LENGTH = AES_KEY_LENGTH * 8;  // Длина симметричного ключа (в битах)
constexpr int MESSAGE_BIT_LENGTH = 259;  // Длина сообщения
constexpr int MESSAGE_LENGTH = MESSAGE_BIT_LENGTH / 8 + (MESSAGE_BIT_LENGTH % 8 ? 1 : 0);    // Длина сообщения в битах
constexpr int ECRYPTED_MESSAGE_LENGTH = MESSAGE_LENGTH - MESSAGE_LENGTH % AES_BLOCK_SIZE +   // Длина зашифрованного сообщения
	(MESSAGE_LENGTH % AES_BLOCK_SIZE ? AES_BLOCK_SIZE : 0);

int main()
{
	setlocale(LC_ALL, "rus");  // Для корректного вывода кирилицы

	AutoSeededRandomPool generator;  // Генератора случайных сивел для crypto++
	ElGamalDecryptor decryptor;  // Расшифровщик Эль-Гамаля

	cout << "Операции на сервере:" << endl;
	cout << "Генерация ключей Эль-Гамаля ..." << endl;
	decryptor.AccessKey().GenerateRandomWithKeySize(generator, ELGAMAL_KEY_BIT_LENGTH);  // Генерация пары ключей Эль-Гамаля

	ElGamalKeys::PrivateKey& private_key = decryptor.AccessKey();  // Получение приватного ключа Эль-Гамаля
	SecByteBlock private_key_bytes = key_to_bytes(private_key);  // Преобразование приватного ключа Эль-Гамаля в байты
	cout << "Приватный ключ Эль-Гамаля:       " << to_string(private_key_bytes) << endl;  // Вывод приватного ключа Эль-Гамаля

	ElGamalEncryptor encryptor(decryptor);  // Шифровщик Эль-Гамаля
	ElGamalKeys::PublicKey& public_key = encryptor.AccessKey();  // Получение публичного ключа Эль-Гамаля
	SecByteBlock public_key_bytes = key_to_bytes(public_key);  // Преобразование публичного ключа Эль-Гамаля в байты
	cout << "Публичный ключ Эль-Гамаля:       " << to_string(public_key_bytes) << endl;  // Вывод публичного ключа Эль-Гамаля
	cout << "Отправка публичного ключа клиенту ..." << endl;

	cout << "\nОперации на клиенте:" << endl;
	SecByteBlock client_symmetric_key(AES_KEY_LENGTH);  // Симметричный ключ (для AES)
	auto encrypted_key_lenght = encryptor.CiphertextLength(client_symmetric_key.size());  // Длина зашифрованного симметричного ключа
	SecByteBlock encrypted_symmetric_key(encrypted_key_lenght);  // Зашифрованный симметричный ключ

	cout << "Генерация симметричного ключа ..." << endl;
	generate_key(client_symmetric_key, AES_KEY_BIT_LENGTH);  // Генерация симметричного ключа (Генератором BBS и проверкой Миллера-Рабина)
	cout << "Симметричный ключ:               " << to_string(client_symmetric_key) << endl;  // Вывод симметричного ключа

	cout << "Шифрование симметричного ключа ..." << endl;
	encryptor.Encrypt(generator, client_symmetric_key, AES_KEY_LENGTH, encrypted_symmetric_key);  // Шифрование симметричного ключа
	cout << "Зашифрованный симметричный ключ: " << to_string(encrypted_symmetric_key) << endl;  // Вывод зашифрованного симметричного ключа

	cout << "Подписывание симмитричного ключа ..." << endl; 
	ElGamalSigner signer(private_key);  // Подписыватель Эль-Гамаля
	auto sign_length = signer.SignatureLength();  // Длина подписи Эль-Гамаля
	SecByteBlock sign(signer.SignatureLength());  // Подпись Эль-Гамаля
	signer.SignMessage(generator, client_symmetric_key, AES_KEY_LENGTH, sign);  // Подписывание симетричного ключа
	cout << "Подпись:                         " << to_string(sign) << endl;  // Вывод подписи
	cout << "Отправка зашифрованного симметричного ключа и подписи на сервер ..." << endl;

	cout << "\nОперации на сервере:" << endl;
	cout << "Расшифровка симметричного ключа..." << endl;
	SecByteBlock server_symmetric_key(AES_KEY_LENGTH);  // Расшифрованный симметричный ключ
	decryptor.Decrypt(generator, encrypted_symmetric_key, encrypted_key_lenght, server_symmetric_key);  // Расшифрование симметричного ключа
	cout << "Симметричный ключ:               " << to_string(server_symmetric_key) << endl;  // Вывод симметричного ключа
	cout << "Проверка подписи клиента..." << endl; 
	ElGamalVerifier verifier(public_key);  // Объект для проверка подписи Эль-Гамаля
	cout << (verifier.VerifyMessage(server_symmetric_key, AES_KEY_LENGTH, sign, sign_length)  // Проверка подписи Эль-Гамаля
		? "Подпись верна." : "Подпись не верна!") << endl;

	cout << endl << "Генерация случайного 128-битного сообщения..." << endl;
	unsigned message_bytes_lenght = 128 / 8;  // Длина сообщения (128 бит)
	SecByteBlock message2(message_bytes_lenght);  // Cообщение
	for (size_t i = 0; i < message_bytes_lenght; i++)  // Проход по сообщению
		message2[i] = rand_bbs();  // Случайный байт
	cout << "Сгенерированное сообщение:       " << to_string(message2) << endl;  // Вывод сообщения
	cout << "Шифрование сообщения ..." << endl;
	AES_KEY aes_key2;  // Ключ для aes шифрования
	AES_set_encrypt_key(server_symmetric_key, AES_KEY_BIT_LENGTH, &aes_key2);  // Установка симметричного ключа
	SecByteBlock encrypted_message2(message_bytes_lenght);  // Зашифрованные байты сообщения
	AES_encrypt(message2, encrypted_message2, &aes_key2);  // AES шифрование
	cout << "\rЗашифрованное сообщение:         " << to_string(encrypted_message2) << endl;  // Вывод зашифрованного сообщения
	cout << "Отправка зашифрованного сообщения клиенту ..." << endl;

	cout << "\nОперации на клиенте:" << endl;
	cout << "Расшифровка сообщения..." << endl;
	SecByteBlock decrypted_message2(message_bytes_lenght);  // Расшифрованное сообщение
	AES_set_decrypt_key(client_symmetric_key, AES_KEY_BIT_LENGTH, &aes_key2);  // Установка симметричного ключа
	AES_decrypt(encrypted_message2, decrypted_message2, &aes_key2);  // Расшифровка сообщения
	cout << "Расшифрованное сообщение:        " << to_string(decrypted_message2) << endl;  // Вывод расшифрованного сообщения


	cout << endl << "Генерация случайного 259-битного сообщения..." << endl;
	SecByteBlock message(MESSAGE_LENGTH);  // Cообщения
	for (int i = MESSAGE_LENGTH - 1; i >= 0; i--)  // Проход по сообщению
		message[i] = (i == 0 ? (rand_bbs() % (1 << (MESSAGE_BIT_LENGTH % 8))) : rand_bbs());  // Случайный байт
	cout << "Сгенерированное сообщение:       " << to_string(message) << endl;  // Вывод сообщения
	cout << "Генерация вектора инициализации..." << endl;
	SecByteBlock server_init_vector(AES_BLOCK_SIZE);  // Вектор инициализация сервера
	SecByteBlock client_init_vector(AES_BLOCK_SIZE);  // Вектор инициализация клиента
	for (size_t i = 0; i < AES_BLOCK_SIZE; i++)  // Проход по байтам ветора инициализации
	{
		auto byte = rand_bbs();  // Случайный байт
		server_init_vector[i] = byte;  // Установка байта в вектор инициализация сервера
		client_init_vector[i] = byte;  // Установка байта в вектор инициализация клиента
	}
	cout << "Вектор инициализации:            " << to_string(server_init_vector) << endl;  // Вывод вектора инициализация
	cout << "Шифрование сообщения ..." << endl;  

	AES_KEY aes_key;  // Ключ для aes шифрования
	SecByteBlock encrypted_message(ECRYPTED_MESSAGE_LENGTH);  // Зашифрованный байты сообщения
	AES_set_encrypt_key(server_symmetric_key, AES_KEY_BIT_LENGTH, &aes_key);  // Установка ключа шифрования
	AES_cbc_encrypt(message, encrypted_message, MESSAGE_LENGTH, &aes_key, server_init_vector, AES_ENCRYPT);  // AES CBC шифрование
	cout << "\rЗашифрованное сообщение:         " << to_string(encrypted_message) << endl;  // Вывод зашифрованного сообщения
	cout << "Отправка зашифрованного сообщения клиенту ..." << endl;

	cout << "\nОперации на клиенте:" << endl;
	cout << "Расшифровка сообщения..." << endl;
	SecByteBlock decrypted_message(MESSAGE_LENGTH);  // Расшифрованное сообщение
	AES_set_decrypt_key(server_symmetric_key, AES_KEY_BIT_LENGTH, &aes_key);  // Установка ключа дешифровки
	AES_cbc_encrypt(encrypted_message, decrypted_message, MESSAGE_LENGTH, &aes_key, client_init_vector, AES_DECRYPT);  // AES CBC дешифровка
	cout << "Расшифрованное сообщение:        " << to_string(decrypted_message) << endl;  // Вывод расшифрованного сообщения
	return 0;
}
