from configparser import ConfigParser

from Crypto.Util.number import getStrongPrime, getRandomRange, inverse, isPrime, GCD, size
from Crypto.Hash import SHA256


class ElGamalKeyGen:

    """
    Class defining an ElGamal keygen.

    Includes:
        generate new key method CryptGenKey
        generate new session key method CryptRefreshSessionKey

        export key method CryptExportKey
        import key method CryptImportKey
    """

    def __init__(self, key_size: int):

        """
        :param key_size: key length in bits. must be a multiple of 128 and >= 1024.

        :raises: ValueError
        """

        if key_size < 1024:
            raise ValueError('Insecure bit length, required 1024 or more.')

        if key_size % 128 != 0:
            raise ValueError('Incorrect key size, must be a multiple of 128.')

        self.KeySize = key_size

    def CryptGenKey(self):

        """
        Randomly generate a fresh, new ElGamal key.
        The key will be safe for use for both encryption and signature.

        :return: ElGamal key: object, session key: int.
        """

        obj = ElGamal()

        obj.KeySize = self.KeySize
        obj.p = int(getStrongPrime(self.KeySize))

        while True:
            # Choose a square residue, it will generate a cyclic group of order (p - 1) / 2
            obj.g = pow(int(getRandomRange(2, obj.p)), 2, obj.p)

            # Avoiding g = {1, 2} because of Bleichenbacher's attack
            if obj.g == 1 or obj.g == 2:
                continue

            # Discarding g if it divides p - 1 because of the attack described in HAC
            if (obj.p - 1) % obj.g == 0:
                continue

            # Inverse of g must not divide p - 1 because of Khadir's attack
            if (obj.p - 1) % int(inverse(obj.g, obj.p)) == 0:
                continue

            break

        obj.x = int(getRandomRange(2, obj.p - 1))
        obj.y = pow(obj.g, obj.x, obj.p)

        k = int(getStrongPrime(self.KeySize - 128, obj.p))

        return obj, k

    def CryptRefreshSessionKey(self, obj):

        """
        A function to refresh session key.

        :param obj: ElGamal key object.

        :return: session key: int.
        """

        return int(getStrongPrime(self.KeySize - 128, obj.p))

    @staticmethod
    def CryptExportKey(obj, secret_filename: str, public_filename: str):

        """
        A function that exports an existing key into .ini file.
        Exported key can be used later via CryptImportKey function.

        :param obj: ElGamal key object.
        :param secret_filename: path to .ini secret file.
        :param public_filename: path to .ini public file.

        :raises: NameError
        """

        if '.ini' not in secret_filename or '.ini' not in public_filename:
            raise NameError('Key must be stored in .ini file.')

        public_config = ConfigParser()

        public_config.add_section('PUBLIC')
        public_config.set('PUBLIC', 'y', str(obj.y))
        public_config.set('PUBLIC', 'g', str(obj.g))
        public_config.set('PUBLIC', 'p', str(obj.p))

        with open(public_filename, 'w') as configfile:
            public_config.write(configfile)

        secret_config = ConfigParser()

        secret_config.add_section('PRIVATE')
        secret_config.set('PRIVATE', 'x', str(obj.x))

        password = input('Set password for secret file:\n >')
        h = int(SHA256.new(password.encode('utf-8')).hexdigest(), 16)

        secret_config.set('PRIVATE', 'pwdhash', str(h))

        with open(secret_filename, 'w') as configfile:
            secret_config.write(configfile)

    @staticmethod
    def CryptImportKey(secret_filename: str, public_filename: str):

        """
        :param secret_filename: path to .ini secret file.
        :param public_filename: path to .ini public file.

        :return: ElGamal key object, fresh session key.

        :raises: NameError, ValueError
        """

        if '.ini' not in secret_filename or '.ini' not in public_filename:
            raise NameError('Key must be stored in .ini file.')

        obj = ElGamal()

        public_config = ConfigParser()
        secret_config = ConfigParser()

        public_config.read(public_filename)
        secret_config.read(secret_filename)

        obj.y = public_config.getint('PUBLIC', 'y')
        obj.g = public_config.getint('PUBLIC', 'g')
        obj.p = public_config.getint('PUBLIC', 'p')

        password = input(f'Enter password for {secret_filename}:\n >')
        h = int(SHA256.new(password.encode('utf-8')).hexdigest(), 16)

        if h != secret_config.getint('PRIVATE', 'pwdhash'):
            raise ValueError('Invalid password.')

        obj.x = secret_config.getint('PRIVATE', 'x')

        if isPrime(obj.p) != 1:
            raise ValueError('Invalid ElGamal key component: p.')
        if pow(obj.g, obj.p - 1, obj.p) != 1:
            raise ValueError('Invalid ElGamal key component: g.')
        if not 1 < obj.x < obj.p - 1:
            raise ValueError('Invalid ElGamal key component: x.')
        if obj.y != pow(obj.g, obj.x, obj.p):
            raise ValueError('Invalid ElGamal key component: y.')

        obj.KeySize = size(obj.p)

        k = int(getStrongPrime(obj.KeySize - 128, obj.p))

        return obj, k


class ElGamal(object):

    """
    Class defining an ElGamal key object and system functionality.

    Includes:
        init and delete methods
        method to get key parameter CryptGetKeyParam
        method to get public part of ElGamal key CryptGetUserKey
        method to destroy key

        ElGamal encryption scheme: CryptEncrypt, CryptDecrypt
        ElGamal signature scheme: CryptCreateHash, CryptSighHash, CryptVerifySignature

        and helper methods:
            CryptEncode, CryptDecode
    """

    def __init__(self):

        """
        :feature y, part of pubkey. y = g^xmodp.
        :feature g, part of pubkey. g - primitive root modulo p.
        :feature p, part of pubkey. strong prime number.
        :feature x, secret key. 1 < x < p - 1.
        :feature KeySize, bit length of ElGamal key.
        """

        self.y = None
        self.g = None
        self.p = None
        self.x = None

        self.KeySize = None

    def __del__(self):
        return None

    def CryptGetKeyParam(self):

        """
        A function to get info about key.KeySize
        """

        print(f'key bit length - {self.KeySize}')

    def CryptGetUserKey(self):

        """
        A function to share a public part of existing key.

        :return: ElGamal key object, with not initialized key.x.
        """

        obj   = ElGamal()

        obj.y = self.y
        obj.g = self.g
        obj.p = self.p

        obj.KeySize = self.KeySize

        return obj

    def CryptDestroyKey(self):

        """
        A function to destroy the ElGamal key object.
        """

        return self.__del__()

    def CryptEncrypt(self, message: str or int, session_key: int):

        """
        A function to encrypt the plaintext.

        :param message: a string or int with content to be encrypted.
        :param session_key: integer that satisfies ElGamal session key condition.

        :return: a, b - parts of ElGamal cryptosystem ciphertext.

        :raises: ValueError, TypeError
        """

        if GCD(self.p - 1, session_key) != 1:
            raise ValueError('Invalid session key.')
        if type(message) is not str and type(message) is not int:
            raise TypeError('Invalid plaintext format.')

        encoded = self.CryptEncode(message) if type(message) is str else message

        if encoded >= self.p:
            raise ValueError('Text too long for encrypt.')

        a = int(pow(self.g, session_key, self.p))
        b = int((pow(self.y, session_key, self.p) * encoded) % self.p)

        return a, b

    def CryptDecrypt(self, ciphertext: tuple):

        """
        A function to decrypt the ciphertext.

        :param ciphertext: tuple contains parts of ElGamal ciphertext.

        :return: decrypted plaintext.

        :raises: AttributeError, ValueError
        """

        if self.x is None:
            raise AttributeError('Private key not available in this object.')

        if ciphertext[0] >= self.p or ciphertext[1] >= self.p:
            raise ValueError('Invalid ciphertext.')

        encoded = int((pow(ciphertext[0], self.p - self.x - 1, self.p) * ciphertext[1]) % self.p)

        if encoded >= self.p:
            raise ValueError('Text too long for decrypt.')

        return self.CryptDecode(encoded)

    def CryptSignHash(self, message: str or int, session_key: int):

        """
        A function to sign the plaintext.

        :param message: a string or int with content to be signed.
        :param session_key: integer that satisfies ElGamal session key condition.

        :return: r, s - parts of ElGamal cryptosystem signature.

        :raises: AttributeError, ValueError, TypeError
        """

        if self.x is None:
            raise AttributeError('Private key not available in this object.')

        if GCD(self.p - 1, session_key) != 1:
            raise ValueError('Invalid session key.')
        if type(message) is not str and type(message) is not int:
            raise TypeError('Invalid plaintext format.')

        m = self.CryptCreateHash(message)

        r = int(pow(self.g, session_key, self.p))
        s = int((m - self.x * r) * int(inverse(session_key, self.p - 1)) % (self.p - 1))

        return r, s

    def CryptVerifySignature(self, plaintext: str, signature: tuple):

        """
        A function to verify the signature.

        :param plaintext: a string with content that was signed.
        :param signature: tuple contains parts of ElGamal signature.

        :return: True if signature verified, False otherwise.

        :raises: ValueError
        """

        if not 0 < signature[0] < self.p:
            raise ValueError('Invalid signature.')
        if not 0 < signature[1] < self.p - 1:
            raise ValueError('Invalid signature.')

        m = self.CryptCreateHash(plaintext)

        s1 = pow(self.y, signature[0], self.p) * pow(signature[0], signature[1], self.p) % self.p
        s2 = pow(self.g, m, self.p)

        return s1 == s2

    @staticmethod
    def CryptEncode(plaintext: str):

        """
        A method to encode plaintext into integer.

        :param plaintext: a string with content to encode.

        :return: int representation of plaintext.

        :raises: UnicodeError
        """

        return int.from_bytes(plaintext.encode('utf-8'), 'little')

    @staticmethod
    def CryptDecode(encoded: int):

        """
        A method to decode integer into plaintext.

        :param encoded: int representation of plaintext.

        :return: a string with decoded content.

        :raises: UnicodeError
        """

        return encoded.to_bytes((encoded.bit_length() + 7) // 8, 'little').decode('utf-8')

    @staticmethod
    def CryptCreateHash(plaintext: str):

        """
        A function that provides SHA256 hash function for ElGamal signature scheme.

        :param plaintext: hash function input

        :return: integer form of hash function hex-output.

        :raises: UnicodeError
        """

        return int(SHA256.new(plaintext.encode('utf-8')).hexdigest(), 16)
