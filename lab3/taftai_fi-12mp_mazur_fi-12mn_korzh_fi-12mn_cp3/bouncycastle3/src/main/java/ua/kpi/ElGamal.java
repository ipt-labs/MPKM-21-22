package ua.kpi;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import java.security.*;

public class ElGamal {

    private final SecureRandom random = new SecureRandom();
    private final String digits = "0123456789abcdef";

    private final Cipher cipher;
    private final KeyPairGenerator kpGen;

    private final int EL_GAMAL_TEST_PLAIN_TEXT_LENGTH = 80;
    private final int EL_GAMAL_TEST_KEY_LENGTH = 2048 / 8;

    public ElGamal() throws NoSuchPaddingException, NoSuchAlgorithmException, NoSuchProviderException {
        cipher = Cipher.getInstance("ElGamal", "BC");
        kpGen = KeyPairGenerator.getInstance("ElGamal", "BC");
    }

    public enum Padding {

        NoPadding("NoPadding"),
        PKCS1Padding("PKCS1Padding"),
        OAEPWithMD5AndMGF1Padding("OAEPWithMD5AndMGF1Padding"),
        OAEPWithSHA1AndMGF1Padding("OAEPWithSHA1AndMGF1Padding"),
        OAEPWithSHA224AndMGF1Padding("OAEPWithSHA224AndMGF1Padding"),
        OAEPWithSHA256AndMGF1Padding("OAEPWithSHA256AndMGF1Padding"),
        OAEPWithSHA384AndMGF1Padding("OAEPWithSHA384AndMGF1Padding"),
        OAEPWithSHA512AndMGF1Padding("OAEPWithSHA512AndMGF1Padding"),
        OAEPWithSHA3224AndMGF1Padding("OAEPWithSHA3-224AndMGF1Padding"),
        OAEPWithSHA3256AndMGF1Padding("OAEPWithSHA3-256AndMGF1Padding"),
        OAEPWithSHA3384AndMGF1Padding("OAEPWithSHA3-384AndMGF1Padding"),
        OAEPWithSHA3512AndMGF1Padding("OAEPWithSHA3-512AndMGF1Padding"),
        ISO97961Padding("ISO9796-1Padding");

        private final String value;

        Padding(String value) {
            this.value = value;
        }

        public String getValue() {
            return value;
        }
    }


    public KeyPair keyPair(int keySize, SecureRandom random) throws Exception {

        kpGen.initialize(keySize, random);
        return kpGen.generateKeyPair();
    }

    public byte[] dec(byte[] cipherText, PrivateKey key) throws Exception {
        cipher.init(Cipher.DECRYPT_MODE, key);
        return cipher.doFinal(cipherText);
    }

    public byte[] enc(byte[] input, PublicKey key) throws Exception {
        cipher.init(Cipher.ENCRYPT_MODE, key);
        return cipher.doFinal(input);
    }

    public void testElGamal() throws Exception {
        int times = 50;
        ElGamal elGamal = new ElGamal();
        long encTime = 0L;
        long decTime = 0L;
        long tempTime;

        for (int i = 0; i < times; i++) {
            byte[] pt = generateRandomBytes(EL_GAMAL_TEST_PLAIN_TEXT_LENGTH);
            KeyPair key = keyPair(EL_GAMAL_TEST_KEY_LENGTH * 8, random);
            tempTime = System.currentTimeMillis();
            byte[] ct = enc(pt, key.getPublic());
            encTime += System.currentTimeMillis() - tempTime;
            tempTime = System.currentTimeMillis();
            byte[] ptDec = elGamal.dec(ct, key.getPrivate());
           // System.out.println("Equals " + Arrays.equals(pt, ptDec));
            decTime += System.currentTimeMillis() - tempTime;
        }
        System.out.printf("ElGamal (%s bytes text, %s bytes key) encryption time (seconds): %.6f%n", EL_GAMAL_TEST_PLAIN_TEXT_LENGTH, EL_GAMAL_TEST_KEY_LENGTH, ((double) encTime / (times * 1000)));
        System.out.printf("ElGamal (%s bytes text, %s bytes key) decryption time (seconds): %.6f%n", EL_GAMAL_TEST_PLAIN_TEXT_LENGTH, EL_GAMAL_TEST_KEY_LENGTH, ((double) decTime / (times * 1000)));
    }

    private byte[] generateRandomBytes(int length) {
        byte[] bytesArray = new byte[length];
        random.nextBytes(bytesArray);
        return bytesArray;
    }
}
