#include "pch.h"
#include <chrono>

#include "rsa.h"
#include "sha.h"
#include "filters.h"
#include "files.h"
#include "osrng.h"
#include "SecBlock.h"
#include "cryptlib.h"

#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>


using namespace std;
int RSA_SHA(int key_size, string plain) {
    try
    {
        using namespace CryptoPP;

        ////////////////////////////////////////////////
        // Generate keys
        AutoSeededRandomPool rng;

        InvertibleRSAFunction parameters;
        parameters.GenerateRandomWithKeySize(rng, key_size);

        RSA::PrivateKey privateKey(parameters);
        RSA::PublicKey publicKey(parameters);

        string cipher, recovered;
        ////////////////////////////////////////////////
        // Encryption
        RSAES_OAEP_SHA_Encryptor e(publicKey);

        StringSource(plain, true,
            new PK_EncryptorFilter(rng, e,
                new StringSink(cipher)
            ) // PK_EncryptorFilter
        ); // StringSource

       ////////////////////////////////////////////////
       ////////////////////////////////////////////////

       ////////////////////////////////////////////////
       // Decryption
        //RSAES_OAEP_SHA_Decryptor d(privateKey);

        //StringSource(cipher, true,
        //    new PK_DecryptorFilter(rng, d,
        //        new StringSink(recovered)
        //    ) // PK_EncryptorFilter
        //); // StringSource

        //assert(plain == recovered);
    }
    catch (CryptoPP::Exception& e)
    {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
    }
}

int main(int argc, char* argv[])
{
    int key_size = 1024;
    auto start = std::chrono::high_resolution_clock::now();
    int times = 1000;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int size_of_plain = 8;
    
    for (int i = 0; i < times; i++) {

        int stringLength = sizeof(alphanum) - 1;
        string plain_text;

        for (unsigned int i = 0; i < size_of_plain; ++i)
        {
            plain_text += alphanum[rand() % stringLength];

        }

        RSA_SHA(key_size, plain_text);


    }
    
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() / times << " s\n";
    return 0;
}

