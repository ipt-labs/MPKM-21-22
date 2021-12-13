# from GaloisField import GaloisField
# from EllipticCurve import EllipticCurve
from DigitalSignature import DigitalSignature


class User:
    def __init__(self, ec, base_point):
        self._ec = ec
        self._base_point = base_point#self._ec.get_base_point()
        self._ds = DigitalSignature(self._ec, self._base_point)
        self._private_key = 0
        self._public_key = 0


    def generate_keypair(self):
        self._private_key, self._public_key = self._ds.get_keypair()


    def get_public_key(self):
        return self._public_key


    def sign_message(self, message, signature_length=512):
        return self._ds.sign(message, self._private_key, signature_length)


    def verify_signature(self, message, signature, public_key, signature_length=512):
        return self._ds.verify_signature(message, signature, public_key, signature_length)
