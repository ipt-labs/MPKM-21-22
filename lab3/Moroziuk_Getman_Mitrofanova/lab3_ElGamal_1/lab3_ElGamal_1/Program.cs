using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Security.Cryptography;

namespace lab3_ElGamal_1
{
    class Program
    {
        static void Main(string[] args)
        {
            ElGamal elgamal = new ElGamal();
            elgamal.GenerateKeys(8);

            string M_String = "A7A72";
            BigInteger M = CryptoMath.HexToBigInteger(M_String);

            BigInteger [] cipher = elgamal.Encryption(M);

            BigInteger decryptedText = elgamal.Decryption(cipher);

            Console.WriteLine(Equals(M, decryptedText));
        }
    }
}