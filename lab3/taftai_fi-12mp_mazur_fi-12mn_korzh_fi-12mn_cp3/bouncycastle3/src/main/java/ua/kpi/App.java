package ua.kpi;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

import javax.crypto.Cipher;
import java.security.*;

/**
 * Hello world!
 *
 */
public class App 
{

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

    public static void main(String[] args) throws Exception {
        (new ElGamal()).testElGamal();
    }
}
