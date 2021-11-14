from sympy import mod_inverse

class EllipticCurvePoint:
    def __init__(self, x: int, y: int, a: int, p: int):
        self.__x = x
        self.__y = y
        self.__a = a
        self.__mod = p


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
            return self.__x == other.__x and self.__y == -other.__y 
        return NotImplemented


    def __add__(self, other):
        if isinstance(other, EllipticCurvePoint):
            alpha = 0
            if self.__is_inverse(other):
                return EllipticCurvePoint(0, 0, self.__a, self.__mod)

            if self == other:
                alpha = (3 * self.__x * self.__x + self.__a) * mod_inverse(2 * self.__y, self.__mod)
            else:
                alpha = (self.__y - other.__y) * mod_inverse(self.__x - other.__x, self.__mod) 

            x = (alpha * alpha - self.__x - other.__x) % self.__mod
            y = (self.__y + alpha * (x - self.__x)) % self.__mod

            return EllipticCurvePoint(x, y, self.__a, self.__mod)
        return NotImplemented


    def __mul__(self, num: int):
        if isinstance(num, int):
            if num == 0 or self == EllipticCurvePoint(0, 0, self.__a, self.__mod):
                return EllipticCurvePoint(0, 0, self.__a, self.__mod)

            result = self
            for i in bin(num)[3:]:
                if i == '1':
                    result = result + result
                result = result + self
            return result
        return NotImplemented

if __name__ == '__main__':
    pass