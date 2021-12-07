from GaloisField import GaloisField
from EllipticCurve import EllipticCurve

gf = GaloisField(m=179, l=4, j=2, k=1)
ec = EllipticCurve(gf)

print(ec)

x, y = ec.get_base_point()
print(hex(x))
print(hex(y))
