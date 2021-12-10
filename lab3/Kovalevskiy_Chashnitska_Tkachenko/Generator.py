from GOST28147_89 import GOST28147_89
from time import time
import pyautogui


class Generator:
    def __init__(self, key):
        self._key = key
        self._cipher_gost = GOST28147_89()
        self._s = self.get_s()
        self._D = int(time()) & 0xFFFFFFFFFFFFFFFF
        self._I = self._cipher_gost.encrypt(self._D, self._key)


    def get_s(self):
        start = time()
        result = 0
        while time() - start < 3:
            point = pyautogui.position()
            x, y = point[0], point[1]
            result ^= self._cipher_gost.encrypt(x ^ y, self._key)
        return result


    def get_random_bit(self):
        x = self._cipher_gost.encrypt(self._I ^ self._s, self._key)
        self._s = self._cipher_gost.encrypt(self._I ^ x, self._key)
        return x & 0b1


    def get_random_64_bit(self):
        result = 0
        for i in range(64):
            result |= self.get_random_bit() << i
        return result


    def get_randint(self, min, max):
        bit_length = len(bin(max)) - 2
        result = 0
        while result < min:
            for i in range(bit_length):
                result |= self.get_random_bit() << i
        return result % max



if __name__ == '__main__':
    key = [0xFFFFFFFF, 0x12345678, 0x00120477, 0x77AE441F, 0x81C63123, 0x99DEEEEE, 0x09502978, 0x68FA3105]
    g = Generator(key)
    for i in range(100):
        print(g.get_randint(5, 23))
