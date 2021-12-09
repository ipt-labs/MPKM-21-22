import random


class GaloisField:
    def __init__(self, m, l, j, k):
        self._m = m
        self._l = l
        self._j = j
        self._k = k
        self._poly = (1 << m) + (1 << l) + (1 << j) + (1 << k) + 1
        self._primitive_ord = (1 << m) - 1


    def __repr__(self):
        return f'''m={self._m},
            irreducible polinomial: x^{self._m} + x^{self._l} + x^{self._j} + x^{self._k},
            primitive ord={hex(self._primitive_ord)}'''


    def get_random(self, start=0, stop=None):
        if stop != None:
            return random.randint(start, stop)
        else:
            return random.randint(start, self._primitive_ord)


    def add_elements(self, x, y):
        return x ^ y


    def multiply_elements(self, x, y):
        result = 0
        while x and y:
            if y & 1 == 1:
                result ^= x
            y //= 2
            carry = x >> (self._m - 1)
            x = (x << 1) & self._primitive_ord
            if carry == 1:
                x ^= self._primitive_ord & self._poly
        return result


    def square_element(self, x):
        return self.multiply_elements(x, x)


    def pow(self, a, n):
        result = 1
        while n > 0:
            if n & 1 == 1:
                result = self.multiply_elements(a, result)
            a = self.square_element(a)
            n //= 2
        return result


    def get_inverse_element(self, x):
        return self.pow(x, (1 << self._m) - 2)


    def divide_element(self, x, y):
        return self.multiply_elements(x, self.get_inverse_element(y))


    def calculate_trace(self, x):
        result = x
        c = self._m - 1
        while c != 0:
            result = self.add_elements(x, self.square_element(result))
            c -= 1
        return result


    def calculate_half_trace(self, x):
        result = x
        c = (self._m - 1) >> 1
        while c != 0:
            result = self.add_elements(self.pow(result, 4), x)
            c -= 1
        return result


    def solve_quadratic_equation(self, u, w):
        if u == 0:
            z = self.pow(w, 1 << (self._m - 1))
            return z, 1
        if w == 0:
            return 0, 2
        v = self.multiply_elements(w, self.square_element(self.get_inverse_element(u)))
        if self.calculate_trace(v) == 1:
            return 0, 0
        ht = self.calculate_half_trace(v)
        return self.multiply_elements(ht, u), 2
