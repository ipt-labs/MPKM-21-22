package ua.kpi;

import javax.crypto.Cipher;
import java.security.*;

public class RSA {

    public KeyPair keyPair(int keySize, SecureRandom random) throws Exception {
        KeyPairGenerator kpGen = KeyPairGenerator.getInstance("RSA", "BC");
        kpGen.initialize(keySize, random);
        return kpGen.generateKeyPair();
    }

    public byte[] dec(byte[] cipherText, PrivateKey key) throws Exception {

        Cipher cipher = Cipher.getInstance("RSA/NONE/PKCS1Padding", "BC");
        cipher.init(Cipher.DECRYPT_MODE, key);
        return cipher.doFinal(cipherText);
    }

    public byte[] enc(byte[] input, PublicKey key) throws Exception {
        Cipher cipher = Cipher.getInstance("RSA/NONE/PKCS1Padding", "BC");
        cipher.init(Cipher.ENCRYPT_MODE, key);
        return cipher.doFinal(input);
    }
}
