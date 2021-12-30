using System;
using System.Numerics;
using System.Security.Cryptography;

namespace lab2
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=================================================================================================================");
            
            var lenArr = new int[] { 32, 64, 128 };
            foreach(var len in lenArr)
            {

                var bytesSeed = new byte[len];
                var seed = new BigInteger(Generators.GenerateRandomByteSeed(4)); 

                var rndGenerator = RandomNumberGenerator.Create();
                var rndStart = DateTime.Now;
                rndGenerator.GetBytes(bytesSeed);
                var rndEnd = DateTime.Now;

                var lehmerStart = DateTime.Now;
                var lehmer = Generators.LehmerHigh(seed, len);
                var lehmerEnd = DateTime.Now;

                var bbsStart = DateTime.Now;
                var bbs = Generators.BBSBytes(seed, len);
                var bbsEnd = DateTime.Now;

                Console.WriteLine($"------------------------------------\nGenerator: LehmerHigh\nResult: {new BigInteger(lehmer).ToString("X")}\nTime: {lehmerEnd - lehmerStart}");
                Console.WriteLine($"------------------------------------\nGenerator: RandomNumberGenerator\nResult: {new BigInteger(bytesSeed).ToString("X")}\nTime: {rndEnd - rndStart}");
                Console.WriteLine($"------------------------------------\nGenerator: BBS Bytes\nResult: {new BigInteger(bbs).ToString("X")}\nTime: {bbsEnd - bbsStart}\n\n");

            }

            //------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            ////var prime = Generators.GeneratePrime(32);
            var prime = 6601;
            Console.WriteLine($"Prime number: {prime}\nTest results:");
            var fermatStart = DateTime.Now;
            var fermat = Tests.FermatTest(prime);
            var fermatEnd = DateTime.Now;

            var ssStart = DateTime.Now;
            var ss = Tests.SolovayStrassenTest(prime);
            var ssEnd = DateTime.Now;

            var mrStart = DateTime.Now;
            var mr = Tests.MillerRabinTest(prime);
            var mrEnd = DateTime.Now;

            Console.WriteLine($"------------------------------------\nTest: Fermat\nResult: {fermat}\nTime: {fermatEnd-fermatStart}");
            Console.WriteLine($"------------------------------------\nTest: Solovay-Strassen\nResult: {ss}\nTime: {ssEnd - ssStart}");
            Console.WriteLine($"------------------------------------\nTest: Miller-Rabin\nResult: {mr}\nTime: {mrEnd - mrStart}");

        }
    }
}
