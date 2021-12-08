from GaloisField import GaloisField
from EllipticCurve import EllipticCurve
from DigitalSignature import DigitalSignature

gf = GaloisField(m=179, l=4, j=2, k=1)
ec = EllipticCurve(gf)

x, y = ec.get_base_point()

ds = DigitalSignature(ec, (x, y))

private, public = ds.get_keypair()

message = 'hello'
print(message)

signed_message = ds.sign(message, private, 512)

print(signed_message[0])
print(hex(signed_message[1]))

print(ds.verify_signature(*signed_message, public, 512))
