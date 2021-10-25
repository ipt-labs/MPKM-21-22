package ua.kpi;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class AES {


    public byte[] encryptionAES256_32EAX(byte[] input, byte[] keyBytes, IvParameterSpec ivSpec) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
        Cipher cipher = Cipher.getInstance("AES/EAX/NoPadding", "BC");
        cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec);

        byte[] cipherText = new byte[cipher.getOutputSize(input.length)];
        int ctLength = cipher.update(input, 0, input.length, cipherText, 0);
        cipher.doFinal(cipherText, ctLength);
        return cipherText;
    }

    public byte[] decryptionAES256_32EAX(byte[] cipherText, byte[] keyBytes, IvParameterSpec ivSpec) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
        Cipher cipher = Cipher.getInstance("AES/EAX/NoPadding", "BC");
        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);

        byte[] plainText = new byte[cipherText.length];
        int ptLength = cipher.update(cipherText, 0, cipherText.length, plainText, 0);
        cipher.doFinal(plainText, ptLength);
        return plainText;
    }

    public byte[] encryptionAES256_32CFB(byte[] input, byte[] keyBytes, IvParameterSpec ivSpec) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
        Cipher cipher = Cipher.getInstance("AES/CFB/NoPadding", "BC");
        cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec);

        byte[] cipherText = new byte[cipher.getOutputSize(input.length)];
        int ctLength = cipher.update(input, 0, input.length, cipherText, 0);
        cipher.doFinal(cipherText, ctLength);
        return cipherText;
    }

    public byte[] decryptionAES256_32CFB(byte[] cipherText, byte[] keyBytes, IvParameterSpec ivSpec) throws Exception {
        SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
        Cipher cipher = Cipher.getInstance("AES/CFB/NoPadding", "BC");
        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);

        byte[] plainText = new byte[cipherText.length];
        int ptLength = cipher.update(cipherText, 0, cipherText.length, plainText, 0);
        cipher.doFinal(plainText, ptLength);
        return plainText;
    }
}
