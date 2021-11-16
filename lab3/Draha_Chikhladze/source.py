def read(path):
    with open(path, 'r') as file:
        text = file.read()
        return text

def write(path, text):
    with open(path, 'w') as file:
        file.write(text)


def norm(x, pol):
    bx = bin(x)[2:]
    bpol = bin(pol)[2:]

    while len(bx) >= len(bpol):
        shift = len(bx) - len(bpol)
        x ^= pol << shift
        bx = bin(x)[2:]
    return x


def mod(x, pol):
        bx = bin(x)[2:]
        bpol = bin(pol)[2:]

        while x >= pol:
            shift = len(bx) - len(bpol)
            x ^= pol << shift
            bx = bin(x)[2:]
        return x

if __name__ == '__main__':
    pass