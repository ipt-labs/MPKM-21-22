from source import read
from point import EllipticCurvePoint as Point
import json
import os

class EllipticCurve:
    def __init__(self, parameters_path: str):
        try:
            parameters = json.loads(read(parameters_path))
        except Exception as e:
            raise e
        
        self.__p = int(parameters['p'], 16)
        self.__a = int(parameters['a'], 16)
        self.__b = int(parameters['b'], 16)
        self.__order = int(parameters['order'], 16)
        self.__G = Point(int(parameters['Gx'], 16), int(parameters['Gy'], 16), self.__a, self.__p)
    
    @property
    def p(self):
        return self.__p
    
    @property
    def a(self):
        return self.__a
    
    @property
    def b(self):
        return self.__b

    @property
    def order(self):
        return self.__order
    
    @property
    def G(self):
        return self.__G
    

if __name__ == '__main__':
    path = path = os.path.dirname(os.path.abspath(__file__))
    EllipticCurve(f'{path}/eliptic_curve256.json')