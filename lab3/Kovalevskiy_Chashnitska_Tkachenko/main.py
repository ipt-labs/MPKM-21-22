from GaloisField import GaloisField
from EllipticCurve import EllipticCurve
from User import User

signature_length = 512

gf = GaloisField(m=179, l=4, j=2, k=1)
ec = EllipticCurve(gf)
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
