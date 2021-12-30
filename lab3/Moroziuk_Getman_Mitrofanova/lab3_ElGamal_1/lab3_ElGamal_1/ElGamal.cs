using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.Security;
using System;
using System.Numerics;

namespace lab3_ElGamal_1
{
    class ElGamal: CryptoMath
    {
        private BigInteger x;
        public BigInteger p;
        public BigInteger g;
        public BigInteger y ;
        public void GenerateKeys( int byteLength)
        {
            p = ChoosePrimeNum(byteLength);
            g = GeneratePrimitiveRoot(p);
            x = GenerateBigInteger(new BigInteger(2), p - new BigInteger(2));
            y = BigInteger.ModPow(g, x, p);
            
        }
       public BigInteger[] Encryption(BigInteger M)
        {
            BigInteger k = GenerateBigInteger(new BigInteger(2), p - new BigInteger(2));
            BigInteger a = BigInteger.ModPow(g, k, p);
            BigInteger b_temp = BigInteger.ModPow(y, k, p);
            BigInteger b = BigInteger.Multiply(b_temp, M) % p;
            BigInteger[] pair = new BigInteger[2] { a, b };
            return pair;
        }

        public BigInteger Decryption(BigInteger[] pair)
        {
            BigInteger a = BigInteger.ModPow(pair[0], p - BigInteger.One - x, p);
            return BigInteger.Multiply(pair[1], a) % p;
        }

    }
}