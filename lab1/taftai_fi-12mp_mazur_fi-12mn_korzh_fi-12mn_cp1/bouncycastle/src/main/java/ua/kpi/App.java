package ua.kpi;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import java.security.KeyPair;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.Security;

/**
 * Hello world!
 */
public class App {
    private static final int PLAIN_TEXT_LENGTH = 50_000;
    private static final int RSA_PLAIN_TEXT_LENGTH = 80;
    private static final int HASH_PLAIN_TEXT_LENGTH = 500_000;
    private static final int KEY_LENGTH = 16;
    private static final int RSA_KEY_LENGTH = 2048 / 8;

    static {
        Security.addProvider(new BouncyCastleProvider());
        //  Security.addProvider(new SunProvider());
        Security.setProperty("crypto.policy", "unlimited");

        int maxKeySize = 0;
        try {
            maxKeySize = Cipher.getMaxAllowedKeyLength("AES");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
    }

    private static void testAESEAX() throws Exception {

        int times = 50;
        AES aes = new AES();
        long encTime = 0L;
        long decTime = 0L;
        long tempTime;
        for (int i = 0; i < times; i++) {
            byte[] pt = Utils.generateRandomBytes(PLAIN_TEXT_LENGTH);
            byte[] key = Utils.generateRandomBytes(KEY_LENGTH);
            IvParameterSpec ivSpec = new IvParameterSpec(Utils.generateRandomBytes(1));

            tempTime = System.currentTimeMillis();
            byte[] ct = aes.encryptionAES256_32EAX(pt, key, ivSpec);
            encTime += System.currentTimeMillis() - tempTime;

            tempTime = System.currentTimeMillis();
            aes.decryptionAES256_32EAX(ct, key, ivSpec);
            decTime += System.currentTimeMillis() - tempTime;
        }
        System.out.printf("AES EAX (%s bytes text, %s bytes key) encryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) encTime / (times * 1000)));
        System.out.printf("AES EAX (%s bytes text, %s bytes key) decryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) decTime / (times * 1000)));
    }

    private static void testAESCFB() throws Exception {

        int times = 50;
        AES aes = new AES();
        long encTime = 0L;
        long decTime = 0L;
        long tempTime;
        for (int i = 0; i < times; i++) {
            byte[] pt = Utils.generateRandomBytes(PLAIN_TEXT_LENGTH);
            byte[] key = Utils.generateRandomBytes(KEY_LENGTH);
            IvParameterSpec ivSpec = new IvParameterSpec(Utils.generateRandomBytes(16));

            tempTime = System.currentTimeMillis();
            byte[] ct = aes.encryptionAES256_32CFB(pt, key, ivSpec);
            encTime += System.currentTimeMillis() - tempTime;

            tempTime = System.currentTimeMillis();
            aes.decryptionAES256_32CFB(ct, key, ivSpec);
            decTime += System.currentTimeMillis() - tempTime;
        }
        System.out.printf("AES CFB (%s bytes text, %s bytes key) encryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) encTime / (times * 1000)));
        System.out.printf("AES CFB (%s bytes text, %s bytes key) decryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) decTime / (times * 1000)));
    }

    private static void testSalsa20() throws Exception {

        int times = 50;
        Salsa salsa = new Salsa();
        long encTime = 0L;
        long decTime = 0L;
        long tempTime;
        for (int i = 0; i < times; i++) {
            byte[] pt = Utils.generateRandomBytes(PLAIN_TEXT_LENGTH);
            byte[] key = Utils.generateRandomBytes(KEY_LENGTH);

            tempTime = System.currentTimeMillis();
            byte[] ct = salsa.enc(pt, key);
            encTime += System.currentTimeMillis() - tempTime;

            tempTime = System.currentTimeMillis();
            salsa.enc(ct, key);
            decTime += System.currentTimeMillis() - tempTime;
        }
        System.out.printf("Salsa20 (%s bytes text, %s bytes key) encryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) encTime / (times * 1000)));
        System.out.printf("Salsa20 (%s bytes text, %s bytes key) decryption time (seconds): %.6f%n", PLAIN_TEXT_LENGTH, KEY_LENGTH, ((double) decTime / (times * 1000)));
    }

    private static void testSHA256() throws Exception {

        int times = 50;
        Hash hash = new Hash();
        long encTime = 0L;
        long tempTime;
        for (int i = 0; i < times; i++) {
            byte[] pt = Utils.generateRandomBytes(HASH_PLAIN_TEXT_LENGTH);
            tempTime = System.currentTimeMillis();
            byte[] h = hash.SHA256(pt);
            encTime += System.currentTimeMillis() - tempTime;
        }
        System.out.println(String.format("SHA256 (%s bytes text length) hashing time (seconds): %.6f",
                HASH_PLAIN_TEXT_LENGTH, ((double) encTime / (times * 1000))));
    }

    private static void testRSA() throws Exception {
        int times = 50;
        RSA rsa = new RSA();
        long encTime = 0L;
        long decTime = 0L;
        long tempTime;
        SecureRandom random = new SecureRandom();
        for (int i = 0; i < times; i++) {
            byte[] pt = Utils.generateRandomBytes(RSA_PLAIN_TEXT_LENGTH);
            KeyPair key = rsa.keyPair(RSA_KEY_LENGTH * 8, random);
            tempTime = System.currentTimeMillis();
            byte[] ct = rsa.enc(pt, key.getPublic());
            encTime += System.currentTimeMillis() - tempTime;
            tempTime = System.currentTimeMillis();
            rsa.dec(ct, key.getPrivate());
            decTime += System.currentTimeMillis() - tempTime;
        }
        System.out.printf("RSA (%s bytes text, %s bytes key) encryption time (seconds): %.6f%n", RSA_PLAIN_TEXT_LENGTH, RSA_KEY_LENGTH, ((double) encTime / (times * 1000)));
        System.out.printf("RSA (%s bytes text, %s bytes key) decryption time (seconds): %.6f%n", RSA_PLAIN_TEXT_LENGTH, RSA_KEY_LENGTH, ((double) decTime / (times * 1000)));
    }

    public static void main(String[] args) throws Exception {
        testAESEAX();
        testAESCFB();
        testSalsa20();
        testRSA();
        testSHA256();
    }
}
