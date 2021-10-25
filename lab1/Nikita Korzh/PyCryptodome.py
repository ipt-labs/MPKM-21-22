# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import time
from secrets import token_bytes

import memory_profiler
from Crypto import Random
from Crypto.Cipher import Salsa20
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15

print("--------- Crypto.Cipher.Package ---------")

print("---------       AES(EAX)        ---------")

key = token_bytes(32)
#text_test = open('AES(EAX).txt', 'r')
plaintext1 = "Let me introduce myself. My name is Mariya I am a 20-year-old student from Donetsk. I study at the university in my native town and my future profession is bookkeeping. I live with my parents and my elder sister Lena. We are a friendly family. Lena is 2 ye"
# print(text)


# @memory_profiler.profile
# def encryptEAX(text):
#     cipher = AES.new(key, AES.MODE_EAX)
#     nonce = cipher.nonce
#     ciphertext, tag = cipher.encrypt_and_digest(text.encode('ascii'))
#     return nonce, ciphertext, tag
#
#
# @memory_profiler.profile
# def decryptEAX(nonce, ciphertext, tag):
#     cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
#     plaintext = cipher.decrypt(ciphertext)
#     try:
#         cipher.verify(tag)
#         return plaintext.decode('ascii'), time.time() - start_time
#     except:
#         return False


# print("length plaintext", len(text))
# start_time = time.time()
# nonce, ciphertext, tag = encryptEAX(text)
# print("%s seconds for encryption" % (time.time() - start_time))
#
# start_time = time.time()
# plaintext = decryptEAX(nonce, ciphertext, tag)
# print("%s seconds for decryption" % (time.time() - start_time))

print("---------       AES(CFB)         ---------")

# key = pad(b"nikita", AES.block_size)
# iv = pad(b'myiv', AES.block_size)
#
#
# @memory_profiler.profile
# def encryptCFB(plaintext):
#     data_bytes = bytes(plaintext, 'utf-8')
#     padded_bytes = pad(data_bytes, AES.block_size)
#     AES_obj = AES.new(key, AES.MODE_CFB, iv)
#     ciphertext = AES_obj.encrypt(padded_bytes)
#     return ciphertext
#
#
# @memory_profiler.profile
# def decryptCFB(ciphertext):
#     AES_obj = AES.new(key, AES.MODE_CFB, iv)
#     raw_bytes = AES_obj.decrypt(ciphertext)
#     extracted_bytes = unpad(raw_bytes, AES.block_size)
#     return extracted_bytes
#
#
# print("length plaintext", len(plaintext))
# start_time = time.time()
# ciphertext = encryptCFB(plaintext)
# print("%s seconds for encryption" % (time.time() - start_time))
#
# start_time = time.time()
# plaintext = decryptCFB(ciphertext)
# print("%s seconds for decryption" % (time.time() - start_time))


print("---------       SALSA20         ---------")

# plaintext = b'Attack at dawn'
secret = b'*Thirty-two byte (256 bits) key*'

plaintext = bytes(plaintext1, 'utf-8')


@memory_profiler.profile
def encryptSAlSA(plaintext):
    cipher = Salsa20.new(key=secret)
    msg = cipher.nonce + cipher.encrypt(plaintext)
    return msg


@memory_profiler.profile
def decryptSAlSA(msg):
    msg_nonce = msg[:8]
    ciphertext = msg[8:]
    cipher = Salsa20.new(key=secret, nonce=msg_nonce)
    plaintext = cipher.decrypt(ciphertext)
    return plaintext


print("length plaintext", len(plaintext))
start_time = time.time()
ciphertext = encryptSAlSA(plaintext)
print("%s seconds for encryption" % (time.time() - start_time))

start_time = time.time()
decryptSAlSA(ciphertext)
print("%s seconds for decryption" % (time.time() - start_time))

print("--------- Crypto.HASH.Package ---------")
print("--------- Crypto.HASH256.Package ---------")

h = SHA256.new()
h.update(b'Hello')
print(h.hexdigest())

print("--------- Crypto.PublicKey.Package ---------")
print("--------- Crypto.RSA.Package ---------")


@memory_profiler.profile
def generate_keys():
    moduls_lenght = 256 * 4
    private_key = RSA.generate(moduls_lenght, Random.new().read)
    public_key = private_key.public_key()
    return private_key, public_key


start_time = time.time()
pri, pub = generate_keys()
print("%s seconds" % (time.time() - start_time))
print("%s seconds" % (time.time() - start_time))

print(pri, pub)

private_key = pri.export_key()
public_key = pub.export_key()

print(private_key, "\n\n\n", public_key)

print("--------- Crypto.Signature.Package ---------")

key = RSA.generate(2048)
private_key = key.export_key()
file_out = open("../private.key", "wb")
file_out.write(private_key)
file_out.close()

public_key = key.publickey().export_key()
file_out = open("../public.key", "wb")
file_out.write(public_key)
file_out.close()

message = b'This message is from me'

key = RSA.import_key(open('../private.key').read())
h = SHA256.new(message)


signer = pkcs1_15.new(key)
signature = signer.sign(h)

print(signature.hex())

file_out = open("../signature.pem", "wb")
file_out.write(signature)
file_out.close()

file_out = open("../message.txt", "wb")
file_out.write(message)
file_out.close()

key = RSA.import_key(open('../public.key').read())

file_in = open("../message.txt", "rb")
message = file_in.read()
file_in.close()

file_in = open("../signature.pem", "rb")
signature = file_in.read()
file_in.close()
h = SHA256.new(message)

try:
    pkcs1_15.new(key).verify(h, signature)
    print("The signature is valid.")
except (ValueError, TypeError):
    print("The signature is not valid.")
