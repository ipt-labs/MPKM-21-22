#include "pch.h"

#include "cryptlib.h"
#include "sha.h"
#include "hex.h"
#include "files.h"

#include <iostream>

using namespace std;
using namespace CryptoPP;

void SHA_256() {
    SHA256 hash;

    //cout << "Name: " << hash.AlgorithmName() << endl;
    //cout << "Digest size: " << hash.DigestSize() << endl;
    //cout << "Block size: " << hash.BlockSize() << endl;

    HexEncoder encoder(new FileSink(cout));

    string msg = "Yodasaid,Doordonot.Thereisnotry.";
    string digest;

    hash.Update((const byte*)msg.data(), msg.size());
    digest.resize(hash.DigestSize());
    hash.Final((byte*)&digest[0]);

    //cout << "Message: " << msg << endl;

    //cout << "Digest: ";
    //StringSource(digest, true, new Redirector(encoder));
    //cout << endl;
}

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();
    int times = 10000;
    
    for (int i = 0; i < times; i++) {

        SHA_256();
    }
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() / times << " s\n";
    
    return 0;
}