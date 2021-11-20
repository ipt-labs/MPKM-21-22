from Crypto.Random import random

f = open('randoutput.txt', 'wb')

for i in range(10000):
	f.write(int(random.getrandbits(1024)).to_bytes(128, byteorder = 'big'))
 
f.close()