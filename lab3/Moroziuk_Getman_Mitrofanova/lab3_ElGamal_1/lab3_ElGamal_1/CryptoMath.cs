using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Numerics;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace lab3_ElGamal_1
{
    public class CryptoMath
    {
        static  public BigInteger HexToBigInteger(string hexNum)
        {
            string hex = "0" + hexNum;
            BigInteger p = BigInteger.Parse(hex, NumberStyles.AllowHexSpecifier);
            return p;
        }
        static protected BigInteger GenerateBigInteger(BigInteger min, BigInteger max)
        {
            BigInteger result = BigInteger.Zero;
            do
            {
                Random rng = new Random();
                byte[] bytes = max.ToByteArray();
                rng.NextBytes(bytes);//заполняет элементы указанного массива случайными символами
                result = new BigInteger(bytes);
            }
            while (result >= max || result < min);
            return result;
        }

        static public  bool Test_MR(BigInteger p)
        {
            int k = 35;
            if (p < new BigInteger(2) || p % new BigInteger(2) == BigInteger.Zero)
                return false;
            if (p == new BigInteger(2) || p == new BigInteger(3))
                return true;
            BigInteger t = p - BigInteger.One;
            int s = 0;
            while (t % new BigInteger(2) == BigInteger.Zero)
            {
                t /= 2;
                s += 1;
            }
            for (int i = 0; i <= k; i++)
            {
                BigInteger a = GenerateBigInteger(new BigInteger(2), p - new BigInteger(2));
                BigInteger x = BigInteger.ModPow(a, t, p);
                if (x == BigInteger.One || x == p - BigInteger.One)
                    continue;
                for (int j = 0; j <= s - 1; j++)
                {
                    x = BigInteger.ModPow(x, new BigInteger(2), p);
                    if (x == BigInteger.One)
                        return false;
                    if (x == p - BigInteger.One)
                        break;

                }
                if (x != p - BigInteger.One)
                    return false;
            }
            return true;
        }
        static byte[] RNGCrypto(int byteLength)
        {
            RNGCryptoServiceProvider provider = new RNGCryptoServiceProvider();
            byte[] byteArray = new byte[byteLength];
            provider.GetBytes(byteArray);
            byteArray[byteArray.Length - 1] &= (byte)0x7Fl;
            return byteArray;
        }

        static  protected BigInteger ChoosePrimeNum(int byteLength)
        {
            byte[] bytes = RNGCrypto(byteLength);
            BigInteger p = new BigInteger(bytes);
            while (Test_MR(p) == false)
            {
                Console.WriteLine("The value doesn`t pass the test");
                Console.WriteLine(p.ToString("X"));
                Console.WriteLine();
                Array.Clear(bytes, 0, byteLength - 1);
                bytes = RNGCrypto(byteLength);
                p = new BigInteger(bytes);
            }
            return p;
        }
        public static BigInteger GCD(BigInteger a, BigInteger b, out BigInteger u, out BigInteger v)
        {
            if (a == BigInteger.Zero)
            {
                u = BigInteger.Zero;
                v = BigInteger.One;
                return b;
            }
            BigInteger u1, v1;
            BigInteger d = GCD(b % a, a, out u1, out v1);
            u = v1 - (b / a) * u1;
            v = u1;

            return d;
        }
        static protected BigInteger Obratny(BigInteger a, BigInteger n)
        {
            BigInteger x, y;
            BigInteger g = GCD(a, n, out x, out y);
            if (g != BigInteger.One)
                throw new ArgumentException();
            return (x % n + n) % n;
        }

        public static List<BigInteger> Factorize(BigInteger factorizeNumber)
        {
            var factors = new List<BigInteger>();
            var sqrt = (BigInteger)Math.Sqrt((double)factorizeNumber) + 1;
            factors.Add(factorizeNumber);
            for (var i = 2; i <= sqrt; i++)
            {
                if (factorizeNumber % i != 0) continue;
                factors.Add(i);

                while (factorizeNumber % i == 0)
                {
                    factorizeNumber /= i;
                }
            }

            return factors;
        }
        public  static int GeneratePrimitiveRoot(BigInteger primeNumber)
        {

            var phi = primeNumber - 1;
            var factors = Factorize(phi);

            for (var res = 2; res <= primeNumber; res++)
            {
                var isPrimitive = true;

                for (var i = 0; i < factors.Count && isPrimitive; i++)
                {
                    isPrimitive &= BigInteger.ModPow(res, phi / factors[i], primeNumber) != 1;
                }

                if (isPrimitive)
                {
                    return res;
                }
            }

            return -1;
        }
    }
}