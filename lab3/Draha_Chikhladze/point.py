from operator import invert
from source import mod, norm

class EllipticCurvePoint:
    def __init__(self, x: int, y: int, a: int, b: int, p: int):
        self.__x = x
        self.__y = y
        self.__a = a
        self.__b = b
        self.__p = p

    
    @property
    def x(self):
        return self.__x


    @property
    def y(self):
        return self.__y


    def __eq__(self, other):
        if isinstance(other, EllipticCurvePoint):
            return self.__x == other.__x and self.__y == other.__y
        return NotImplemented


    def __ne__(self, other):
        result = self.__eq__(other)
        if result is NotImplemented:
            return result
        return not result


    def __is_inverse(self, other):
        if isinstance(other, EllipticCurvePoint):
            return other.__x == self.__x and other.__y == self.__x ^ self.__y 
        return NotImplemented

    def __mul_coord(self, x1, x2):
        if x1 == 0 or x2 == 0:
            return 0
        
        bx1 = bin(x1)[2:][::-1]
        indexes = [i for i in range(len(bx1)) if bx1[i] == '1']
        res = 0
        for i in indexes:
            res ^= x2 << i

        return self.__norm(res, self.__p)


    def __norm(self, x, pol):
        return norm(x, pol)


    def __mod(self, x, pol):
        return mod(x, pol)


    def __div(self, divided, divider):
        quotient = 0
        ldivider = len(bin(divider)[2:])
        while divided >= divider:
            ldivided = len(bin(divided)[2:])
            quotient ^= 1 << (ldivided - ldivider)
            divided ^= divider << (ldivided - ldivider)
        return quotient


    def __mod_inverse(self, value, mod):
        v = [0, 1]
        rem = 1
        a = mod

        while rem > 0:
            tmp = self.__div(a, value)
            v[0] ^= self.__mul_coord(v[1], tmp)
            v[0], v[1] = v[1], v[0]
            rem = self.__mod(a, value)
            a = value
            value = rem
        return v[0]


    def __add__(self, other):
        # t = self.__mul_coord(self.y, self.y) ^ self.__mul_coord(self.x, self.y) ^ self.__mul_coord(self.x, self.__mul_coord(self.x, self.x)) ^ self.__mul_coord(self.x, self.x) ^ self.__b
        
        if isinstance(other, EllipticCurvePoint):
            if self.__is_inverse(other):
                return EllipticCurvePoint(0, 0, self.__a, self.__b, self.__p)

            if self.__x == 0 and self.__y == 0:
                return other
            if other.__x == 0 and other.__y == 0:
                return self

            x = 0
            y = 0
            if self == other:
                if self.__x == 0:
                    return EllipticCurvePoint(0, 0, self.__a, self.__b, self.__p)

                tmp = self.__mul_coord(self.__x, self.__x)
                x = tmp ^ self.__mul_coord(
                    self.__b, 
                    self.__mod_inverse(tmp, self.__p)
                )

                y = tmp ^ self.__mul_coord(
                    self.__x ^ self.__mul_coord(self.__y, self.__mod_inverse(self.__x, self.__p)), 
                    x
                ) ^ x
            else:
                tmp = self.__mul_coord(
                        self.__y ^ other.__y, 
                        self.__mod_inverse(self.__x ^ other.__x, self.__p)
                    )
                x = self.__mul_coord(tmp, tmp) ^ tmp ^ self.__x ^ other.__x ^ self.__a

                y = self.__mul_coord(
                    tmp,
                    self.__x ^ x
                ) ^ x ^ self.__y            

            return EllipticCurvePoint(x, y, self.__a, self.__b, self.__p)
        return NotImplemented


    def __mul__(self, num: int):
        if isinstance(num, int):
            if num == 0 or self == EllipticCurvePoint(0, 0, self.__a, self.__b, self.__p):
                return EllipticCurvePoint(0, 0, self.__a, self.__b, self.__p)


            tmp = self
            if num < 0:
                num = abs(num) 
                tmp.__y = self.__x ^ self.__y

            result = EllipticCurvePoint(0, 0, self.__a, self.__b, self.__p)
            itr = bin(num)[2:][::-1]
            for i in itr:
                if i == '1':
                    result = result + tmp
                tmp = tmp + tmp
            return result
        return NotImplemented

if __name__ == '__main__':
    x = int("0x72D867F93A93AC27DF9FF01AFFE74885C8C540420", 16)
    y = int("0x0224A9C3947852B97C5599D5F4AB81122ADC3FD9B", 16)
    p = int("0x800000000000000000000000000000000000000c9", 16)
    A = int("0x1", 16)
    B = int("0x5FF6108462A2DC8210AB403925E638A19C1455D21", 16)
    d = int("0x183F60FDF7951FF47D67193F8D073790C1C9B5A3E", 16)


    # a = EllipticCurvePoint(x, y, A, B, p)
    # b = EllipticCurvePoint(x, y, A, B, p)
    # for i in range(d - 1):
    #     b = b+a
    # print(hex(b.x))
    # print(hex(b.y))
    # bb = a * d

    # tt = 0
    a = EllipticCurvePoint(x, y, A, B, p)
    b = EllipticCurvePoint(x, y, A, B, p)

    # tmp = a + b

    n = 20
    for i in range(n - 1):
        b = b + a

    # t = b.__mul_coord(b.y, b.y) ^ b.__mul_coord(b.x, b.y) ^ b.__mul_coord(b.x, b.__mul_coord(b.x, b.x)) ^ b.__mul_coord(b.x, b.x) ^ b.__b
    tt = 0

    # c = a * n
    print(b == c)
