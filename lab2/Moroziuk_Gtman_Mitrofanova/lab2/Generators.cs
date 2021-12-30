using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;

namespace lab2
{
    class Generators
    {
        public static byte[] GenerateRandomByteSeed(int size)
        {
            Random rnd = new Random();
            byte[] seed = new byte[size];
            byte[] zeros = new byte[size];
            Array.Fill(zeros, (byte)0);
            rnd.NextBytes(seed);
            if (seed.SequenceEqual(zeros))
            {
                seed[seed.Length - 1] = (byte)1;
            }
            return seed;
        }
        public static BigInteger GenerateBigInteger(BigInteger max)
        {
            Random rnd = new Random();
            byte[] maxBytes = max.ToByteArray(true, false);
            byte[] seedBytes = new byte[maxBytes.Length];

            rnd.NextBytes(seedBytes);
            seedBytes[seedBytes.Length - 1] &= (byte)0x7F;
            var seed = new BigInteger(seedBytes);

            while (seed > max || seed < Tests.TWO)
            {
                rnd.NextBytes(seedBytes);
                seed = new BigInteger(seedBytes);
            }

            return seed;
        }
        public static BigInteger GeneratePrime(int byteLength)
        {
            var bytes = GenerateRandomByteSeed(byteLength);
            var num = BigInteger.Abs(new BigInteger(bytes));
            while (!Tests.MillerRabinTest(num))
            {
                bytes = GenerateRandomByteSeed(byteLength);
                num = BigInteger.Abs(new BigInteger(bytes));
            }

            return num;
        }

        public static Tuple<BigInteger, BigInteger> GeneratePrimePair(int byteLength)
        {
            BigInteger p = GeneratePrime(byteLength);
            BigInteger q = GeneratePrime(byteLength);
            while (p.ToByteArray().Length > byteLength || q.ToByteArray().Length > byteLength || BigInteger.Compare(p, q) == 0)
            {
                p = GeneratePrime(byteLength);
                q = GeneratePrime(byteLength);
            }
            return Tuple.Create(p, q);
        }

        public static BigInteger M = new BigInteger(Math.Pow(2, 32));
        public static BigInteger A = new BigInteger(Math.Pow(2, 16) + 1);
        public static BigInteger C = new BigInteger(119);

        public static BigInteger LehmerNext(BigInteger x)
        {
            BigInteger next = (A * x + C) % M;
            if (next.Sign == -1)
            {
                next = next + M;
            }
            return next;
        }

        public static byte[] LehmerHigh(BigInteger seed, int len)
        {
            var next = seed;
            byte[] bytes = new byte[len];
            for (int i = 0; i < len; i++)
            {
                next = LehmerNext(next);
                var nextArr = next.ToByteArray(true, false);
                if (nextArr.Length < 4) bytes[i] = (byte)0;
                else
                {
                    bytes[i] = nextArr[nextArr.Length - 1];
                }

            }
            return bytes;
        }

        static BigInteger pBBS = BigInteger.Parse("D5BBB96D30086EC484EBA3D7F9CAEB07", System.Globalization.NumberStyles.AllowHexSpecifier),
                        qBBS = BigInteger.Parse("425D2B9BFDB25B9CF6C416CC6E37B59C1F", System.Globalization.NumberStyles.AllowHexSpecifier),
                        nBBS = pBBS * qBBS;
        public static byte[] BBSBytes(BigInteger r0, int len)//рандомный биг инт за 4 байта
        {
            if (r0 < new BigInteger(2))
            {
                r0 = new BigInteger(2);
            }
            var rNext = r0;
            byte[] bytes = new byte[len];
            for (int i = 0; i < len; i++)
            {
                rNext = BigInteger.ModPow(rNext, 2, nBBS);
                var rNextArray = rNext.ToByteArray(true, false);
                bytes[i] = rNextArray[0];
            }
            return bytes;
        }
    }
}