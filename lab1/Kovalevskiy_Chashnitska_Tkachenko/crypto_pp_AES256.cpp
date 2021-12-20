#include "pch.h"

#include <chrono>
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <string>

#include "hrtimer.h"

using namespace CryptoPP;
using namespace std;
int AES_CBC(string plain_text) {

    string encoded;
    AutoSeededRandomPool prng;
    HexEncoder encoder(new StringSink(encoded));

    SecByteBlock key(AES::MAX_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());


    string plain = plain_text;
    string cipher, recovered;

    //cout << "plain text: " << plain << endl;

    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (const Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    //cout << "key: ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    //cout << endl;

    //cout << "iv: ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    //cout << endl;

    //cout << "cipher text: ";
    encoder.Put((const byte*)&cipher[0], cipher.size());
    encoder.MessageEnd();
    //cout << endl;

    /*********************************\
    \*********************************/

    //try
    //{
      //CBC_Mode< AES >::Decryption d;
      //d.SetKeyWithIV(key, key.size(), iv);

      //StringSource s(cipher, true,
         // new StreamTransformationFilter(d,
              //new StringSink(recovered)
          //) // StreamTransformationFilter
      //); // StringSource

        //cout << "recovered text: " << recovered << endl;
      //}
    //catch (const Exception& e)
    //{
        //cerr << e.what() << std::endl;
        //exit(1);
    //}
}

int main(int argc, char* argv[])
{

    auto start = chrono::high_resolution_clock::now();
    int times = 10000;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int size_of_plain = 32;
    ThreadUserTimer timer;
    timer.StartTimer();
    double elapsedTimeInSeconds;
    for (int i = 0; i < times; i++) {

        int stringLength = sizeof(alphanum) - 1;
        string plain_text;

        for (unsigned int i = 0; i < size_of_plain; ++i)
        {
            plain_text += alphanum[rand() % stringLength];

        }
        
        AES_CBC(plain_text);


    }
    elapsedTimeInSeconds = timer.ElapsedTimeAsDouble();
    cout << elapsedTimeInSeconds/times << endl;
    
        auto finish = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = finish - start;
        cout << "Elapsed time: " << elapsed.count() / times << " s\n";

        return 0;
}
