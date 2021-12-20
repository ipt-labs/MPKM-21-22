package ua.kpi;

import org.bouncycastle.crypto.digests.SHA256Digest;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.math.Primes;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import java.security.*;
import java.util.Arrays;

/**
 * Hello world!
 */
public class App {
    private static final int KEY_LENGTH = 16;
    private static final int RSA_KEY_LENGTH = 2048 / 8;

    static {
        Security.addProvider(new BouncyCastleProvider());
        Security.setProperty("crypto.policy", "unlimited");

        int maxKeySize = 0;
        try {
            maxKeySize = Cipher.getMaxAllowedKeyLength("AES");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
    }

    private static void generateRSAKeyPair(int keySize, SecureRandom random) throws Exception {
        KeyPairGenerator kpGen = KeyPairGenerator.getInstance("RSA", "BC");
        kpGen.initialize(keySize, random);
        KeyPair keyPair = kpGen.generateKeyPair();
        System.out.println("keyPair private: \n" + keyPair.getPrivate());
        System.out.println("keyPair  public: \n" + keyPair.getPublic());
    }


    private static void generateKey(int keySize, SecureRandom random) throws Exception {
        KeyGenerator keyGen = KeyGenerator.getInstance("AES", "BC");
        keyGen.init(keySize, random);

        Key key = keyGen.generateKey();

        System.out.println("key " + key.getAlgorithm() + "  " + key.getFormat());
        System.out.println("key: " + Arrays.toString(key.getEncoded()));
    }


    private static void prime(int keySize, SecureRandom random) throws Exception {
        Primes.STOutput prime = Primes.generateSTRandomPrime(new SHA256Digest(), keySize, Utils.generateRandomBytes(10));
        System.out.println("Shawe-Taylor Random_Prime Routine " + prime.getPrime().toString(16));
        System.out.println("Miller-Rabin Probabilistic Primality Test isMRProbablePrime " +
                Primes.isMRProbablePrime(prime.getPrime(), random, 4));
    }


    public static void main(String[] args) throws Exception {

        SecureRandom rand = new SecureRandom();
        generateKey(KEY_LENGTH, rand);
        generateRSAKeyPair(RSA_KEY_LENGTH, rand);
        prime(RSA_KEY_LENGTH * 8, rand);
    }
}
