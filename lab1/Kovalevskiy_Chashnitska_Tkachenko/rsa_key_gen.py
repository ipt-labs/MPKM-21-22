from OpenSSL import crypto
from memory_profiler import profile

@profile
def rsa_key_gen_pair():
    k = crypto.PKey()
    key = k.generate_key(crypto.TYPE_RSA, 2048)
    
    return key 

rsa_key_gen_pair()