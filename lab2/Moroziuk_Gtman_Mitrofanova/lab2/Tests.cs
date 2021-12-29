using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;

namespace lab2
{
    class Tests
    {
        public static BigInteger TWO = new BigInteger(2);
        public static BigInteger THREE = new BigInteger(3);
        public static BigInteger FOUR = new BigInteger(4);

        static BigInteger JacobiSymbol(BigInteger a, BigInteger n)
        {
            BigInteger r, temp;
            BigInteger t = BigInteger.One;
            while (a != 0)
            {
                while (a % 2 == 0)
                {
                    a /= 2;
                    r = n % 8;
                    if (r == 3 || r == 5)
                    {
                        t = -t;
                    }
                }
                temp = a;
                a = n;
                n = temp;
                if (a % 4 == n % 4 && n % 4 == 3)
                {
                    t = -t;
                }
                a %= n;

            }
            if (n == 1)
            {
                return t;
            }
            else
            {
                return 0;
            }
        }
        public static bool FermatTest(BigInteger num)
        {
            if (num == TWO || num == THREE)
            {
                return true;
            }
            if (num < TWO || num % TWO == BigInteger.Zero)
            {
                return false;
            }
            var a = Generators.GenerateBigInteger(num - 1);
            return BigInteger.ModPow(a, num - 1, num) == BigInteger.One;
        }

        public static bool SolovayStrassenTest(BigInteger num)
        {
            if (num == TWO || num == THREE)
            {
                return true;
            }
            if (num < TWO || num % TWO == BigInteger.Zero)
            {
                return false;
            }
            var a = Generators.GenerateBigInteger(num - 1);
            while(BigInteger.GreatestCommonDivisor(a,num) > BigInteger.One)
            {
                a = Generators.GenerateBigInteger(num - 1);
            }
            var x = JacobiSymbol(a, num);

            return BigInteger.ModPow(a, (num - BigInteger.One)/TWO, num) == x;
        }

        public static bool MillerRabinTest(BigInteger num, int k = 30) //робили по псевдокоду з вікіпедії
        {
            if (num == TWO || num == THREE)
            {
                return true;
            }
            if (num < TWO || num % TWO == BigInteger.Zero)
            {
                return false;
            }

            BigInteger d = num - BigInteger.One;
            int s = 0;

            while (d % TWO == BigInteger.Zero)
            {
                d /= TWO;
                s++;
            }

            for (int i = 0; i < k; i++)
            {
                var a = Generators.GenerateBigInteger(num - TWO);
                var x = BigInteger.ModPow(a, d, num);
                if (x == BigInteger.One || x == num - BigInteger.One)
                {
                    continue;
                }
                for (int j = 0; j < s; j++)
                {
                    x = BigInteger.ModPow(x, TWO, num);
                    if (x == BigInteger.One)
                    {
                        return false;
                    }
                    if (x == num - BigInteger.One)
                    {
                        break;
                    }
                }
                if (x != num - BigInteger.One)
                {
                    return false;
                }
            }

            return true;
        }

    }
}
