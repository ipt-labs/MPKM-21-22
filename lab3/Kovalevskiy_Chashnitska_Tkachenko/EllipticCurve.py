class EllipticCurve:
    def __init__(self, gf, A=1, B=0x4A6E0856526436F2F88DD07A341E32D04184572BEB710,
                 n=0x3FFFFFFFFFFFFFFFFFFFFFFB981960435FE5AB64236EF):
        self.__gf = gf
        self.__A = A
        self.__B = B
        self.__n = n
        self.__infinity = (0, 0)

    def is_on_curve(self, p):
        if p == self.__infinity:
            return True
        x, y = p
        left_part = self.__gf.add_elements(
            self.__gf.square_element(y),
            self.__gf.multiply_elements(x, y)
        )
        right_part = self.__gf.add_elements(
            self.__gf.add_elements(self.__gf.pow(x, 3),
                self.__gf.multiply_elements(
                    self.__A, self.__gf.square_element(x)
                )
            ), self.__B
        )
        return left_part == right_part


    def get_random_point(self):
        while True:
            u = self.__gf.get_random()
            w = self.__gf.add_elements(
                self.__gf.add_elements(
                    self.__gf.pow(u, 3),
                    self.__gf.multiply_elements(
                        self.__A, self.__gf.square_element(u)
                    )
                ), self.__B
            )
            z, k = self.__gf.solve_quadratic_equation(u, w)
            if k > 0:
                break
        return u, z


    def negate_point(self, point):
        x, y = point
        return x, self.__gf.add_elements(x, y)


    def add_points(self, p_1, p_2):
        if p_1 == self.__infinity:
            return p_2
        if p_2 == self.__infinity:
            return p__1
        if p_1 == p_2:
            return self.double_point(p_1)
        if p_2 == self.negate_point(p_1):
            return self.__infinity

        x_1, y_1 = p_1
        x_2, y_2 = p_2
        sum_x = self.__gf.add_elements(x_1, x_2)
        mu = self.__gf.divide_element(
            self.__gf.add_elements(y_1, y_2),
            sum_x
        )
        x_3 = self.__gf.add_elements(
            self.__gf.add_elements(
                self.__gf.add_elements(
                    self.__gf.square_element(mu), mu
                ),
                sum_x
            ),
            self.__A
        )
        y_3 = self.__gf.add_elements(
            self.__gf.add_elements(
                self.__gf.multiply_elements(
                    mu, self.__gf.add_elements(x_1, x_3)
                ), x_3
            ), y_1
        )

        return x_3, y_3


    def double_point(self, p):
        x, y = p
        mu = self.__gf.add_elements(x, self.__gf.divide_element(y, x))
        x_2 = self.__gf.add_elements(
            self.__gf.add_elements(
                self.__gf.square_element(mu), mu
            ), self.__A
        )
        y_2 = self.__gf.add_elements(
            self.__gf.square_element(x),
            self.__gf.multiply_elements(
                self.__gf.add_elements(mu, 1), x_2
            )
        )

        return x_2, y_2


    def multiply_point(self, p, n):
        q = self.__infinity
        while n > 0:
            if n & 1 == 1:
                q = self.add_points(q, p)
            p = self.double_point(p)
            n //= 2
        return q


    def get_base_point(self):
        while True:
            p = self.get_random_point()
            r = self.multiply_point(p, self.__n)
            if r == self.__infinity:
                return p


from GaloisField import GaloisField
gf = GaloisField(m=179, l=4, j=2, k=1)
ec = EllipticCurve(gf)

x, y = ec.get_base_point()
print(hex(x))
print(hex(y))
