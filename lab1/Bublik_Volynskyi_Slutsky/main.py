import time
import matplotlib.pyplot as plt

from struct import pack

from Crypto.Cipher import AES
from Crypto.Cipher import Blowfish
from Crypto.Cipher import CAST
from Crypto.Cipher import DES3

from Crypto.Random import get_random_bytes


def Rijndael_encr(size):
    aes_key = get_random_bytes(32)
    aes_cipher = AES.new(aes_key, AES.MODE_EAX)

    start_time = time.time()
    aes_cipher.encrypt_and_digest(get_random_bytes(size))
    return time.time() - start_time


def Blowfish_encr(size):
    bs = Blowfish.block_size
    blow_key = get_random_bytes(16)
    blow_cipher = Blowfish.new(blow_key, Blowfish.MODE_CBC)

    plaintext = get_random_bytes(size)
    plen = bs - len(plaintext) % bs

    padding = [plen] * plen
    padding = pack('b' * plen, *padding)

    start_time = time.time()
    blow_cipher.iv + blow_cipher.encrypt(plaintext + padding)
    return time.time() - start_time


def CAST5_encr(size):
    cast_key = get_random_bytes(16)
    cast_cipher = CAST.new(cast_key, CAST.MODE_OPENPGP)

    start_time = time.time()
    cast_cipher.encrypt(get_random_bytes(size))
    return time.time() - start_time


def DES3_encr(size):
    while True:
        try:
            des_key = DES3.adjust_key_parity(get_random_bytes(24))
            break
        except ValueError:
            pass

    des_cipher = DES3.new(des_key, DES3.MODE_CFB)

    start_time = time.time()
    des_cipher.iv + des_cipher.encrypt(get_random_bytes(size))
    return time.time() - start_time


data_length = [i for i in range(10, 1000001)]

Rjindael_time = []
Blowfish_time = []
CAST5_time = []
TripleDES_time = []

for size in data_length:
    Rjindael_time.append(Rijndael_encr(size))
    Blowfish_time.append(Blowfish_encr(size))
    CAST5_time.append(CAST5_encr(size))
    TripleDES_time.append(DES3_encr(size))

fig = plt.figure()
ax = plt.subplot(111)

plt.xlabel('Data Length in Bytes')
plt.ylabel('Seconds')
plt.title('PyCrypto Ciphers: Absolute Time by Data Length')

plt.xlim([10, 1000000])

ax.plot(data_length, Rjindael_time, label='Rjindael', color=(255 / 255, 113 / 255, 113 / 255))
ax.plot(data_length, Blowfish_time, label='Blowfish', color=(241 / 255, 241 / 255, 43 / 255))
ax.plot(data_length, CAST5_time, label='CAST5', color=(134 / 255, 134 / 255, 221 / 255))
ax.plot(data_length, TripleDES_time, label='3DES', color=(255 / 255, 196 / 255, 46 / 255))

box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.1, box.width, box.height * 0.9])

ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.2),
          fancybox=True, shadow=False, ncol=4)

plt.savefig('PyCrypto.png', dpi=400, bbox_inches='tight')
