class EllipticCurve:
    def __init__(self, gf, A=1, B=0x4A6E0856526436F2F88DD07A341E32D04184572BEB710,
                 n=0x3FFFFFFFFFFFFFFFFFFFFFFB981960435FE5AB64236EF):
        self._gf = gf
        self._A = A
        self._B = B
        self._n = n
        self._infinity = (0, 0)


    def __repr__(self):
        return f'''Elliptic curve:
        Galois field:
            {self._gf}
        A={hex(self._A)}
        B={hex(self._B)}
        n={hex(self._n)}'''


    def is_on_curve(self, p):
        if p == self._infinity:
            return True
        x, y = p
        left_part = self._gf.add_elements(
            self._gf.square_element(y),
            self._gf.multiply_elements(x, y)
        )
        right_part = self._gf.add_elements(
            self._gf.add_elements(self._gf.pow(x, 3),
                self._gf.multiply_elements(
                    self._A, self._gf.square_element(x)
                )
            ), self._B
        )
        return left_part == right_part


    def get_random_point(self):
        while True:
            u = self._gf.get_random()
            w = self._gf.add_elements(
                self._gf.add_elements(
                    self._gf.pow(u, 3),
                    self._gf.multiply_elements(
                        self._A, self._gf.square_element(u)
                    )
                ), self._B
            )
            z, k = self._gf.solve_quadratic_equation(u, w)
            if k > 0:
                break
        return u, z


    def negate_point(self, point):
        x, y = point
        return x, self._gf.add_elements(x, y)


    def add_points(self, p_1, p_2):
        if p_1 == self._infinity:
            return p_2
        if p_2 == self._infinity:
            return p__1
        if p_1 == p_2:
            return self.double_point(p_1)
        if p_2 == self.negate_point(p_1):
            return self._infinity

        x_1, y_1 = p_1
        x_2, y_2 = p_2
        sum_x = self._gf.add_elements(x_1, x_2)
        mu = self._gf.divide_element(
            self._gf.add_elements(y_1, y_2),
            sum_x
        )
        x_3 = self._gf.add_elements(
            self._gf.add_elements(
                self._gf.add_elements(
                    self._gf.square_element(mu), mu
                ),
                sum_x
            ),
            self._A
        )
        y_3 = self._gf.add_elements(
            self._gf.add_elements(
                self._gf.multiply_elements(
                    mu, self._gf.add_elements(x_1, x_3)
                ), x_3
            ), y_1
        )

        return x_3, y_3


    def double_point(self, p):
        x, y = p
        mu = self._gf.add_elements(x, self._gf.divide_element(y, x))
        x_2 = self._gf.add_elements(
            self._gf.add_elements(
                self._gf.square_element(mu), mu
            ), self._A
        )
        y_2 = self._gf.add_elements(
            self._gf.square_element(x),
            self._gf.multiply_elements(
                self._gf.add_elements(mu, 1), x_2
            )
        )

        return x_2, y_2


    def multiply_point(self, p, n):
        q = self._infinity
        while n > 0:
            if n & 1 == 1:
                q = self.add_points(q, p)
            p = self.double_point(p)
            n //= 2
        return q


    def get_base_point(self):
        while True:
            p = self.get_random_point()
            r = self.multiply_point(p, self._n)
            if r == self._infinity:
                return p
