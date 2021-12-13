import random
from Crypto.Hash import SHA256


class DigitalSignature:
    def __init__(self, ec, base_point):
        self._ec = ec
        self._base_point = base_point
        self._length = len(bin(self._ec._n)[2:])
        self._mask = (1 << (self._length - 1)) - 1
        self._hash_func = SHA256.new
        # self._generator = generator


    def get_random(self, start=1, stop=None):
        if stop != None:
            return random.randint(start, stop)
            # return self._ec._gf._generator.get_randint(start, stop)
        else:
            return random.randint(start, self._mask)
            # return self._ec._gf._generator.get_randint(start, self._mask)


    def get_private_key(self):
        return self.get_random(start=1)


    def get_public_key(self, d):
        return self._ec.multiply_point(self._ec.negate_point(self._base_point), d)


    def get_keypair(self):
        pk = self.get_private_key()
        return pk, self.get_public_key(pk)


    def get_presignature(self):
        while True:
            e = self.get_random(start=1)
            x, y = self._ec.multiply_point(self._base_point, e)
            if x != 0:
                return e, x


    def sign(self, message, d, signature_length=512):
        if isinstance(message, str):
            message = message.encode('utf-8')
        if signature_length % 16 != 0 or signature_length < 2 * self._length:
            raise RuntimeError('Signature length != 0 mod 16 or < 2L(n)')
        bytes_digest = self._hash_func(message).digest()
        h = int.from_bytes(bytes_digest, byteorder='big') & self._ec._gf._primitive_ord
        # print(f'h={hex(h)}')
        if h == 0:
            h = 1
        while True:
            e, e_x = self.get_presignature()
            r = self._ec._gf.multiply_elements(h, e_x) & self._mask
            if r == 0:
                continue
            s = (e + d * r) % self._ec._n
            if s != 0:
                break
        return message.decode('utf-8'), self.make_signature(r, s, signature_length)


    def make_signature(self, r, s, signature_length):
        return (s << (signature_length >> 1)) ^ r


    def verify_signature(self, message, signature, Q, signature_length=512):
        if isinstance(message, str):
            message = message.encode('utf-8')
        if signature_length % 16 != 0 or signature_length < 2 * self._length:
            raise RuntimeError('Signature length != 0 mod 16 or < 2L(n)')
        bytes_digest = self._hash_func(message).digest()
        h = int.from_bytes(bytes_digest, byteorder='big') & self._ec._gf._primitive_ord
        if h == 0:
            h = 1
        temp_signature_mask = (1 << (signature_length >> 1)) - 1
        r = signature & temp_signature_mask
        s = (signature >> (signature_length >> 1)) & temp_signature_mask
        if not (0 < r < self._ec._n) or not (0 < s < self._ec._n):
            return False
        x, y = self._ec.add_points(
            self._ec.multiply_point(self._base_point, s),
            self._ec.multiply_point(Q, r))
        r_2 = self._ec._gf.multiply_elements(h, x) & self._mask
        return r == r_2
