from GaloisField import GaloisField
from EllipticCurve import EllipticCurve
from User import User

signature_length = 512

# x^431+x^5+x^3+x+1
# m = 431
# l = 5
# j = 3
# k = 1
# A = 1
# B = 0x03CE10490F6A708FC26DFE8C3D27C4F94E690134D5BFF988D8D28AAEAEDE975936C66BAC536B18AE2DC312CA493117DAA469C640CAF3
# n = 0x3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBA3175458009A8C0A724F02F81AA8A1FCBAF80D90C7A95110504CF

# x^163+x^7+x^6+x^3+1
m = 163
l = 7
j = 6
k = 3
A = 1
B = 0x5FF6108462A2DC8210AB403925E638A19C1455D21
n = 0x400000000000000000002BEC12BE2262D39BCF14D



# print(len(bin(n)[2:]))

gf = GaloisField(m=m, l=l, j=j, k=k)
ec = EllipticCurve(gf, A=A, B=B, n=n)
base_point = ec.get_base_point()

Alice = User(ec, base_point)
Bob = User(ec, base_point)

Alice.generate_keypair()
Bob.generate_keypair()

alice_message = 'Hello, Bob!'
bob_message = 'Hey!'

signed_message = Alice.sign_message(alice_message, signature_length)
print(f'Alice\'s signed message: {signed_message[0]} {hex(signed_message[1])}')

alice_public_key = Alice.get_public_key()

print(f'Bob verification: {Bob.verify_signature(*signed_message, alice_public_key, signature_length)}')

signed_message = Bob.sign_message(bob_message, signature_length)
print(f'Bob\'s signed message: {signed_message[0]} {hex(signed_message[1])}')

bob_public_key = Bob.get_public_key()

print(f'Alice verification: {Alice.verify_signature(*signed_message, bob_public_key, signature_length)}')

Eva = User(ec, base_point)
Eva.generate_keypair()
eva_message = 'Go kill yourself, Bob!'
signed_message = Eva.sign_message(eva_message, signature_length)

print(f'Eva\'s signed message: {signed_message[0]} {hex(signed_message[1])}')

eva_public_key = Eva.get_public_key()

print(f'Bob verification Eva\'s message via Alice\'s public key: {Bob.verify_signature(*signed_message, alice_public_key, signature_length)}')
print(f'Bob verification Eva\'s message via Eva\'s public key: {Bob.verify_signature(*signed_message, eva_public_key, signature_length)}')
