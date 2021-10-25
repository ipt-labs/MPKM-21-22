package ua.kpi;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

public class Salsa {

    public byte[] enc(byte[] input, byte[] keyBytes) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "Salsa20");
        Cipher cipher = Cipher.getInstance("Salsa20", "BC");
        cipher.init(Cipher.ENCRYPT_MODE, key);
        byte[] cipherText = new byte[cipher.getOutputSize(input.length)];
        int ctLength = cipher.update(input, 0, input.length, cipherText, 0);
        cipher.doFinal(cipherText, ctLength);
        return cipherText;
    }

    public byte[] dec(byte[] cipherText, byte[] keyBytes) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "Salsa20");
        Cipher cipher = Cipher.getInstance("Salsa20", "BC");
        cipher.init(Cipher.DECRYPT_MODE, key);

        byte[] plainText = new byte[cipherText.length];
        int ptLength = cipher.update(cipherText, 0, cipherText.length, plainText, 0);
        cipher.doFinal(plainText, ptLength);
        return plainText;
    }
}
