from GaloisField import GaloisField
from EllipticCurve import EllipticCurve
from DigitalSignature import DigitalSignature

class API:
    def __init__(self, ec, base_point):
        self.__ec = ec
        self.__base_point = base_point
        self.__ds = DigitalSignature(self.__ec, self.__base_point)
        self.__private_key = 0
        self.__public_key = 0


    def get_base_point(self):
        return self.__base_point


    def generate_private_key(self):
        self.__private_key = self.__ds.get_private_key()
        # print(hex(self.__private_key))


    def generate_public_key(self):
        if self.__private_key == 0:
            raise Exception()
        self.__public_key = self.__ds.get_public_key(self.__private_key)
        # print(hex(self.__public_key[0]), hex(self.__public_key[1]))

    def export_private_key(self, filename):
        if self.__private_key == 0:
            raise Exception()
        with open(filename, 'w') as f:
            f.write(hex(self.__private_key))


    def export_public_key(self, filename):
        if self.__public_key == 0:
            raise Exception()
        with open(filename, 'w') as f:
            f.write(f'{hex(self.__public_key[0])}\n')
            f.write(hex(self.__public_key[1]))


    def import_private_key(self, filename):
        if self.__private_key != 0:
            self.del_private_key()
        with open(filename, 'r') as f:
            self.__private_key = int(f.read(), 16)
        # print(hex(self.__private_key))


    def import_public_key(self, filename):
        if self.__public_key != 0:
            self.del_public_key()
        with open(filename, 'r') as f:
            key = f.readlines()
        self.__public_key = (int(key[0].replace('\n', ''), 16), int(key[1].replace('\n', ''), 16))
        # print(hex(self.__public_key[0]))


    def del_private_key(self):
        del self.__private_key
        self.__private_key = 0


    def del_public_key(self):
        del self.__public_key
        self.__public_key = 0


    def sign(self, msg, signature_length=512):
        if self.__private_key == 0:
            raise Exception()
        return self.__ds.sign(msg, self.__private_key, signature_length)


    def verify(self, message, signature, public_key, signature_length=512):
        if self.__public_key == 0:
            raise Exception()
        return self.__ds.verify_signature(message, signature, public_key, signature_length)


    def get_public_key(self):
        return self.__public_key


if __name__ == '__main__':
    #parameters

    # x^163+x^7+x^6+x^3+1
    m = 163
    l = 7
    j = 6
    k = 3
    A = 1
    B = 0x5FF6108462A2DC8210AB403925E638A19C1455D21
    n = 0x400000000000000000002BEC12BE2262D39BCF14D

    gf = GaloisField(m=m, l=l, j=j, k=k) #, generator=generator)
    ec = EllipticCurve(gf, A=A, B=B, n=n)
    base_point = ec.get_base_point()

    api1 = API(ec, base_point)
    api1.generate_private_key()
    api1.generate_public_key()
    api1.export_private_key('private.pem')
    api1.export_public_key('public.pem')
    api1.import_private_key('private.pem')
    api1.import_public_key('public.pem')

    message = 'Hello world!'
    signature = api1.sign(message)[1]
    print(message, hex(signature))

    api1_public_key = api1.get_public_key()


    api2 = API(ec, base_point)
    api2.generate_private_key()
    api2.generate_public_key()
    print(api2.verify(message, signature, api1_public_key))

    # print(api.verify(message, signature))
