#include "KeyGeneration.h"
#include "MillerRabin.h"

#include <stdio.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/err.h>
#include <openssl/applink.c>
#include <openssl/aes.h>
#include <cryptopp/elgamal.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nr.h>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace CryptoPP;

X509* generate_x509(EVP_PKEY* pkey)  // Генерация сертифика x509
{
	X509* x509 = X509_new();  // Вывделение памяти
	if (!x509)  // При ошибке
	{
		std::cerr << "Unable to create X509 structure." << std::endl;  // Вывод ошибки
		return NULL;
	}
	ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);  // Формат x509 (номер сертификата)
	X509_gmtime_adj(X509_get_notBefore(x509), 0);  // Формат x509
	X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);  // Формат x509
	X509_set_pubkey(x509, pkey);  // Установка публичного ключа
	X509_NAME* name = X509_get_subject_name(x509);  // Получение данных сертификата
	X509_NAME_add_entry_by_txt(name, "U", MBSTRING_ASC, (unsigned char*)"UK", -1, -1, 0);  // Установка данных сертификата
	X509_NAME_add_entry_by_txt(name, "SF", MBSTRING_ASC, (unsigned char*)"SecFirst", -1, -1, 0);  // Название компании
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"Server", -1, -1, 0);  // Сервер
	X509_set_issuer_name(x509, name);  // Установка данных сертификата
	if (!X509_sign(x509, pkey, EVP_sha1()))  // Подпись сертификата
	{
		std::cerr << "Error signing certificate." << std::endl;  // Ошибка
		X509_free(x509);
		return NULL;
	}
	return x509;  // Вернуть сертификат x509
}

PKCS12* generate_pkcs12(EVP_PKEY* key, X509* main, STACK_OF(X509)* cacert)  // Генерация сертификата PKSC
{
	PKCS12* pkcs12;
	if ((pkcs12 = PKCS12_new()) == NULL)  // Выделение памяти
	{
		std::cerr << "Error creating PKCS12 structure.\n";  // Ошибка
		return NULL;
	}
	pkcs12 = PKCS12_create("4758vjhkbkerjhuyeduCUTYVB++jht%^^&", "SecFirst", key, main, cacert, 0, 0, 0, 0, 0);  // Создание сертификата
	if (pkcs12 == NULL)  // Если ошибка
	{
		std::cerr << "Error generating a valid PKCS12 certificate.\n";  // Ошибка
		return NULL;
	}
	return pkcs12;  // Вернуть сертификат
}

bool save_pkcs12(PKCS12* pkcs12, const char* path)  // Сохранить сертификат PKSC
{
	FILE* pkcs12file;  // Файл
	if (!(pkcs12file = fopen(path, "w")))  // Открытие для записи
	{
		std::cerr << "Error cant open pkcs12 certificate file for writing.\n";  // Вывод ошибки
		return false;
	}
	auto bytes = i2d_PKCS12_fp(pkcs12file, pkcs12);  // Запись сертификата в файл
	if (bytes <= 0)
	{
		std::cerr << "Error writing PKCS12 certificate.\n";  // Ошибка
		fclose(pkcs12file);  // Закрытие файла
		return false;
	}
	fclose(pkcs12file);  // Закрытие файла
	return true;
}

PKCS12* load_pkcs12(const char* path)  // Загрузка сертификата PKSC
{
	FILE* fp;  // Файл
	PKCS12* p12;  // Сертификат

	if (!(fp = fopen(path, "rb")))  // Открытие файла для чтения
	{
		std::cerr << "Error opening file '" << path << "'\n";  // Ошибка
		return NULL;
	}
	p12 = d2i_PKCS12_fp(fp, NULL);  // Чтение сертификата
	fclose(fp);  // Закртие файла
	return p12;  // Вернуть сертификат
}

SecByteBlock key_to_bytes(ElGamalKeys::PrivateKey& key)  // Преобразование приватного ключа Эль-Гамаля в байты
{
	Integer exponent = key.GetPrivateExponent();  // Получение приватного элемента
	unsigned byteCount = exponent.ByteCount();  // Получение количества байт
	SecByteBlock buffer(byteCount);  // Байты приватного ключа 
	for (size_t i = 0; i < byteCount; i++)  // Проход по ключу
		buffer[i] = exponent.GetByte(byteCount - i - 1);  // Получение байта
	return buffer;  // Возврат байт
}

EVP_PKEY* generate_key()  // Генерация ключа для сертификата
{
	EVP_PKEY* pkey = EVP_PKEY_new();  // Выделение памяти
	if (!pkey)  // При ошибке
	{
		std::cerr << "Unable to create EVP_PKEY structure." << std::endl;  // Вывод ошибки
		return NULL;
	}
	RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);  // Генерация ключа 2048 бит
	if (!EVP_PKEY_assign_RSA(pkey, rsa))  // Установка ключа в pkey
	{
		std::cerr << "Unable to generate RSA key." << std::endl;  // Ошибка
		EVP_PKEY_free(pkey);
		return NULL;
	}
	return pkey;  // Возврат ключа
}

SecByteBlock key_to_bytes(ElGamalKeys::PublicKey& key)  // Преобразование публичного ключа Эль-Гамаля в байты
{
	Integer exponent = key.GetPublicElement();  // Получение публшичного элемента
	unsigned byteCount = exponent.ByteCount();  // Получение количества байт
	SecByteBlock buffer(byteCount);  // Байты публичного ключа 
	for (size_t i = 0; i < byteCount; i++)  // Проход по ключу
		buffer[i] = exponent.GetByte(byteCount - i - 1);  // Получение байта
	return buffer;  // Возврат байт
}

string to_string(byte* key, size_t lenght)  // Преобразование массива байт в hex строку
{
	stringstream stream;  // Поток
	for (size_t i = 0; i < lenght; i++)  // Проход по байтам
		stream << hex << setw(2) << setfill('0') <<  // Вывод байта в поток
			uppercase << (unsigned int)key[i];
	return stream.str();  // Преобразование в строку
}

string to_string(SecByteBlock bytes)  // Вывод массива байт в виде hex числа
{
	return to_string(bytes, bytes.size());  // Возврат hex числа из массива
}

typedef NR<CryptoPP::SHA256>::Signer ElGamalSigner;  // Класс модифицированной подписи Эль-Гамаля 
typedef NR<CryptoPP::SHA256>::Verifier ElGamalVerifier;  // Класс проверки подписи Эль-Гамаля 

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
	cout << "Генерация сертификата ..." << endl;
	EVP_PKEY* pkey = generate_key();  // Генерация ключа для сертификата
	X509* x509 = generate_x509(pkey);  // Генерация сертификата x509
	STACK_OF(X509)* cacert = sk_X509_new_null();  // Генерация stack сертификата x509
	PKCS12* pkcs12 = generate_pkcs12(pkey, x509, cacert);  // Генерация сертификата pkcs12
	cout << "Отправка сертификата клиенту..." << endl;
	if (!save_pkcs12(pkcs12, "certificate.p12"))  // Сохранение сертификата
	{
		std::cout << "Ошибка отправки сертификата!\n";  // Ошибка
		return 1;
	}
	X509_free(x509);  // Освобождение памяти
	PKCS12_free(pkcs12);  // Освобождение памяти

	cout << "\nОперации на клиенте:" << endl;
	pkcs12 = load_pkcs12("certificate.p12");  // Загрузка сертификата
	if (!pkcs12)
	{
		std::cerr << "Ошибка получения файла сертификата!\n";  // Ошибка
		return 1;
	}
	pkey = NULL;
	if (!PKCS12_parse(pkcs12, "4758vjhkbkerjhuyeduCUTYVB++jht%^^&", &pkey, &x509, &cacert))  // Парсинг сертификата
	{
		std::cerr << "Ошибка парсинга файла сертификата!\n";
		return 1;
	}
	PKCS12_free(pkcs12);  // Освобождение памяти
	EVP_PKEY_free(pkey);  // Освобождение памяти
	SecByteBlock client_symmetric_key(AES_KEY_LENGTH);  // Симметричный ключ (для AES)
	auto encrypted_key_lenght = encryptor.CiphertextLength(client_symmetric_key.size());  // Длина зашифрованного симметричного ключа
	SecByteBlock encrypted_symmetric_key(encrypted_key_lenght);  // Зашифрованный симметричный ключ

	cout << "Генерация симметричного ключа ..." << endl;
	generate_key(client_symmetric_key, AES_KEY_BIT_LENGTH);  // Генерация симметричного ключа (Генератором BBS и проверкой Миллера-Рабина)
	cout << "Симметричный ключ:               " << to_string(client_symmetric_key) << endl;  // Вывод симметричного ключа

	cout << "Шифрование симметричного ключа ..." << endl;
	encryptor.Encrypt(generator, client_symmetric_key, AES_KEY_LENGTH, encrypted_symmetric_key);  // Шифрование симметричного ключа
	cout << "Зашифрованный симметричный ключ: " << to_string(encrypted_symmetric_key) << endl;  // Вывод зашифрованного симметричного ключа

	cout << "Подписывание симметричного ключа ..." << endl; 
	ElGamalSigner signer(private_key);  // Подписыватель Эль-Гамаля
	auto sign_length = signer.SignatureLength();  // Длина подписи Эль-Гамаля
	SecByteBlock sign(signer.SignatureLength());  // Подпись Эль-Гамаля
	signer.SignMessage(generator, client_symmetric_key, AES_KEY_LENGTH, sign);  // Подписывание симметричного ключа
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
	cout << "Генерация случайного сообщения..." << endl;
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
