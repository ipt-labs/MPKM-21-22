from system import ElGamalKeyGen


def TestKeyGenVersion():

    print('============================')
    print('\n== GENERATE NEW KEYS FLOW ==\n')
    print('============================')

    # Keygen initialize
    KeyGen = ElGamalKeyGen(2048)

    # Generating keys for Alice, sharing pubkey
    KeyPair1, SessionKey1 = KeyGen.CryptGenKey()
    PubKey1 = KeyPair1.CryptGetUserKey()
    print('Keys for Alice generated.')

    # Generating keys for Bob, sharing pubkey
    KeyPair2, SessionKey2 = KeyGen.CryptGenKey()
    PubKey2 = KeyPair2.CryptGetUserKey()
    print('Keys for Bob generated.\n')

    message1 = 'are you sure the system works?'
    print(f'Alice sending message to Bob: {message1}')

    # Encrypting message using Bob's pubkey
    Encrypted1 = PubKey2.CryptEncrypt(message1, SessionKey1)

    # Decrypting message using Bob's seckey
    print(f'Bob receiving message from Alice: {KeyPair2.CryptDecrypt(Encrypted1)}')

    message2 = 'absolutely sure, lets test signature'
    print(f'\nand sending message to Alice: {message2}')

    # Encrypting message using Alice's pubkey
    Encrypted2 = PubKey1.CryptEncrypt(message2, SessionKey2)

    # Decrypting message using Alice's seckey
    print(f'Alice receiving message from Bob: {KeyPair1.CryptDecrypt(Encrypted2)}')

    message3 = 'signature test'
    print(f'\nand sending signature to Bob: {message3}')
    print(f'Bob receiving signature from Alice: {message3}')

    # Refreshing session key for security purpose
    SessionKey1 = KeyGen.CryptRefreshSessionKey(PubKey1)

    # Signing message using Alice's seckey
    Signature1 = KeyPair1.CryptSignHash(message3, SessionKey1)

    # Verifying signature using Alice's pubkey
    print(f'and verifying Alice\'s signature: {PubKey1.CryptVerifySignature(message3, Signature1)}\n')

    # Exporting Alice's and Bob's keys
    KeyGen.CryptExportKey(KeyPair1, 'Alice/secret.ini', 'Alice_public.ini')
    KeyGen.CryptExportKey(KeyPair2, 'Bob/secret.ini', 'Bob_public.ini')

    # Destroying Alice's keys
    KeyPair1.CryptDestroyKey()
    del SessionKey1
    print('Keys for Alice destroyed.')

    # Destroying Bob's keys
    KeyPair2.CryptDestroyKey()
    del SessionKey2
    print('Keys for Bob destroyed.\n\n')


def TestKeyImportVersion():
    print('=============================')
    print('\n= IMPORT EXISTING KEYS FLOW =\n')
    print('=============================')

    KeyGen      = ElGamalKeyGen(2048)

    # Importing keys for Alice, sharing pubkey
    KeyPair1, SessionKey1 = KeyGen.CryptImportKey('Alice/secret.ini', 'Alice_public.ini')
    PubKey1     = KeyPair1.CryptGetUserKey()
    print('\nKeys for Alice imported.')

    # Importing keys for Bob, sharing pubkey
    KeyPair2, SessionKey2 = KeyGen.CryptImportKey('Bob/secret.ini', 'Bob_public.ini')
    PubKey2     = KeyPair2.CryptGetUserKey()
    print('Keys for Bob imported.\n')

    message1    = 'are you sure the system works?'
    print(f'Alice sending message to Bob: {message1}')

    # Encrypting message using Bob's pubkey
    Encrypted1  = PubKey2.CryptEncrypt(message1, SessionKey1)

    # Decrypting message using Bob's seckey
    print(f'Bob receiving message from Alice: {KeyPair2.CryptDecrypt(Encrypted1)}')

    message2    = 'absolutely sure, lets test signature'
    print(f'\nand sending message to Alice: {message2}')

    # Encrypting message using Alice's pubkey
    Encrypted2  = PubKey1.CryptEncrypt(message2, SessionKey2)

    # Decrypting message using Alice's seckey
    print(f'Alice receiving message from Bob: {KeyPair1.CryptDecrypt(Encrypted2)}')

    message3    = 'signature test'
    print(f'\nand sending signature to Bob: {message3}')
    print(f'Bob receiving signature from Alice: {message3}')

    # Refreshing session key for security purpose
    SessionKey1 = KeyGen.CryptRefreshSessionKey(PubKey1)

    # Signing message using Alice's seckey
    Signature1  = KeyPair1.CryptSignHash(message3, SessionKey1)

    # Verifying signature using Alice's pubkey
    print(f'and verifying Alice\'s signature: {PubKey1.CryptVerifySignature(message3, Signature1)}\n')

    # Destroying Alice's keys
    KeyPair1.CryptDestroyKey()
    del SessionKey1
    print('Keys for Alice destroyed.')

    # Destroying Bob's keys
    KeyPair2.CryptDestroyKey()
    del SessionKey2
    print('Keys for Bob destroyed.\n\n')
