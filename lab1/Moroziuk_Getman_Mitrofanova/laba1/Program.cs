using System;
using System.Numerics;
using System.Collections.Generic;
using System.Linq;

namespace laba1
{
    class Program
    {
        static string HexToBin(string hexString)
        {
            string binString = "";

            for (int i = 0; i < hexString.Length; i++)
            {
                if (hexString[i] == '0') { binString += "0000"; }
                if (hexString[i] == '1') { binString += "0001"; }
                if (hexString[i] == '2') { binString += "0010"; }
                if (hexString[i] == '3') { binString += "0011"; }
                if (hexString[i] == '4') { binString += "0100"; }
                if (hexString[i] == '5') { binString += "0101"; }
                if (hexString[i] == '6') { binString += "0110"; }
                if (hexString[i] == '7') { binString += "0111"; }
                if (hexString[i] == '8') { binString += "1000"; }
                if (hexString[i] == '9') { binString += "1001"; }
                if (hexString[i] == 'A') { binString += "1010"; }
                if (hexString[i] == 'B') { binString += "1011"; }
                if (hexString[i] == 'C') { binString += "1100"; }
                if (hexString[i] == 'D') { binString += "1101"; }
                if (hexString[i] == 'E') { binString += "1110"; }
                if (hexString[i] == 'F') { binString += "1111"; }
            }

            return binString;
        }

        static BigInteger ParseHex(string hexStr)
        {
            string binaryStr = HexToBin(hexStr);
            var res = BigInteger.Zero;
            foreach (char c in binaryStr)
            {
                res <<= 1;
                res += c == '1' ? 1 : 0;
            }

            return res;
        }

        static byte[] GenerateRandomByteSeed(int size)//функция генерации начального заполнения
        {
            Random rnd = new Random();
            byte[] seed = new byte[size];
            byte[] zeros = new byte[size];
            Array.Fill(zeros, (byte)0);
            rnd.NextBytes(seed);
            if (seed.SequenceEqual(zeros))
            {
                seed[3] = (byte)1;
            }
            seed[seed.Length - 1] = 0b00000000;
            return seed;
        }
        static void Main(string[] args)
        {
            var number1 = ParseHex("A12497B017796C126D89EAE27D6A7A75212A12497B017796C126D89EAE27D6A7A752123FFF2167A6767DDCBE67EE673FFF2167A6767DDCBE67EE67");
            var number2 = BigInteger.Parse("A12497B017796C126D89EAE27D6A7A752123FFF2167A6767DDCBE67EE67A12497B017796C126D89EAE27D6A7A752123FFF2167A6767DDCBE67EE67", System.Globalization.NumberStyles.HexNumber);
            
            Console.WriteLine($"input: A12497B017796C126D89EAE27D6A7A752123FFF2167A6767DDCBE67EE67A12497B017796C126D89EAE27D6A7A752123FFF2167A6767DDCBE67EE67");
            Console.WriteLine($"number1: {number1}");
            Console.WriteLine($"number2: {number2}");
            
            
            int[] len_arr = new int[] { 64, 128, 256 };


            foreach(var len in len_arr)
            {
                //-------------------------------------------- негерування великих чисел різної довжини
                var number_1 = new BigInteger(GenerateRandomByteSeed(len));
                var number_2 = new BigInteger(GenerateRandomByteSeed(len));
                Console.WriteLine($"===================================================================================================================================================");
                Console.WriteLine($"\n   Numbers creation:");
                Console.WriteLine($"Byte length: {len}");
                Console.WriteLine($"number1: {number_1.ToString("X")}");
                Console.WriteLine($"number2: {number_2.ToString("X")}");
                //-------------------------------------------- додавання великих чисел різної довжини
                Console.WriteLine($"\n   ----------------------------------------------------------------------");
                Console.WriteLine($"\n   Sum of 2 numbers using method vs operator:");
                var t_start_plus = DateTime.Now;
                var sum_method = BigInteger.Add(number_1, number_2);
                var t_end_plus = DateTime.Now;
                Console.WriteLine($"Method:");
                Console.WriteLine($"Sum: {sum_method.ToString("X")}\nTime: {t_end_plus-t_start_plus}");

                var t_start_plus_op = DateTime.Now;
                var sum_opp = number_1 + number_2;
                var t_end_plus_op = DateTime.Now;
                Console.WriteLine($"Operator:");
                Console.WriteLine($"Sum: {sum_opp.ToString("X")}\nTime: {t_end_plus_op - t_start_plus_op}");
                //-------------------------------------------- віднімання великих чисел різної довжини
                Console.WriteLine($"\n   ----------------------------------------------------------------------");
                Console.WriteLine($"\n   Subtraction of 2 numbers using method vs operator:");
                var t_start_minus = DateTime.Now;
                var sub_method = BigInteger.Subtract(number_1, number_2);
                var t_end_minus = DateTime.Now;
                Console.WriteLine($"Method:");
                Console.WriteLine($"Subtraction: {sum_method.ToString("X")}\nTime: {t_end_minus - t_start_minus}");

                var t_start_minus_op = DateTime.Now;
                var minus_opp = number_1 - number_2;
                var t_end_minus_op = DateTime.Now;
                Console.WriteLine($"Operator:");
                Console.WriteLine($"Subtraction: {sum_opp.ToString("X")}\nTime: {t_end_minus_op - t_start_minus_op}");
                //-------------------------------------------- ділення великих чисел різної довжини
                Console.WriteLine($"\n   ----------------------------------------------------------------------");
                Console.WriteLine($"\n   Division of 2 numbers using method vs operator:");
                var t_start_div = DateTime.Now;
                var div_method = BigInteger.Divide(number_1, number_2);
                var t_end_div = DateTime.Now;
                Console.WriteLine($"Method:");
                Console.WriteLine($"Division: {div_method.ToString("X")}\nTime: {t_end_div - t_start_div}");

                var t_start_div_op = DateTime.Now;
                var div_opp = number_1 / number_2;
                var t_end_div_op = DateTime.Now;
                Console.WriteLine($"Operator:");
                Console.WriteLine($"Division: {div_opp.ToString("X")}\nTime: {t_end_div_op - t_start_div_op}");

                //-------------------------------------------- множення великих чисел різної довжини
                Console.WriteLine($"\n   ----------------------------------------------------------------------");
                Console.WriteLine($"\n   Multiplication of 2 numbers using method vs operator:");
                var t_start_mul = DateTime.Now;
                var mul_method = BigInteger.Multiply(number_1, number_2);
                var t_end_mul = DateTime.Now;
                Console.WriteLine($"Method:");
                Console.WriteLine($"Multiplication: {mul_method.ToString("X")}\nTime: {t_end_mul - t_start_mul}");

                var t_start_mul_op = DateTime.Now;
                var mul_opp = number_1 * number_2;
                var t_end_mul_op = DateTime.Now;
                Console.WriteLine($"Operator:");
                Console.WriteLine($"Multiplication: {mul_opp.ToString("X")}\nTime: {t_end_mul_op - t_start_mul_op}");

                //-------------------------------------------- піднесення до степеня чисел різної довжини
                Console.WriteLine($"\n   ----------------------------------------------------------------------");
                Console.WriteLine($"\n   Power of number using method vs operator:");
                var t_start_pow = DateTime.Now;
                var pow_method = BigInteger.Pow(number_1, 3);
                var t_end_pow = DateTime.Now;
                Console.WriteLine($"Method:");
                Console.WriteLine($"Power: {pow_method.ToString("X")}\nTime: {t_end_pow - t_start_pow}");

                var t_start_pow_op = DateTime.Now;
                var pow_opp = number_1 * number_1 * number_1;
                var t_end_pow_op = DateTime.Now;
                Console.WriteLine($"Operator:");
                Console.WriteLine($"Power: {pow_opp.ToString("X")}\nTime: {t_end_pow_op - t_start_pow_op}");

            }



        }
    }
}
