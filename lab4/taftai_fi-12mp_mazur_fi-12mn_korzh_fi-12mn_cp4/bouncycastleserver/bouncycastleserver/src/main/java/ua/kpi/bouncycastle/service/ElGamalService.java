package ua.kpi.bouncycastle.service;

import org.springframework.stereotype.Service;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import java.security.*;

@Service
public class ElGamalService {

    private final SecureRandom random = new SecureRandom();
    private  Cipher cipher;

    private final int EL_GAMAL_TEST_PLAIN_TEXT_LENGTH = 80;
    private final int EL_GAMAL_TEST_KEY_LENGTH = 2048 / 8;

    public ElGamalService() throws NoSuchPaddingException, NoSuchAlgorithmException, NoSuchProviderException {
        cipher = Cipher.getInstance("ElGamal", "BC");
    }


    public byte[] dec(byte[] cipherText, PrivateKey key) throws Exception {
        cipher.init(Cipher.DECRYPT_MODE, key);
        return cipher.doFinal(cipherText);
    }

    public byte[] enc(byte[] input, PublicKey key) throws Exception {
        cipher.init(Cipher.ENCRYPT_MODE, key);
        return cipher.doFinal(input);
    }

    private final String digits = "0123456789abcdef";

    public String toHex(byte[] data, int length) {
        StringBuilder buf = new StringBuilder();

        for (int i = 0; i != length; i++) {
            int v = data[i] & 0xff;

            buf.append(digits.charAt(v >> 4));
            buf.append(digits.charAt(v & 0xf));
        }

        return buf.toString();
    }

}
