package ua.kpi;

import java.security.SecureRandom;

public class Utils {

    private static final SecureRandom random = new SecureRandom();

    public static byte[] generateRandomBytes(int length) {
        byte[] bytesArray = new byte[length];
        random.nextBytes(bytesArray);
        return bytesArray;
    }
}
