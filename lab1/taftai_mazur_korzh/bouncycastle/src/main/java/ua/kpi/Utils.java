package ua.kpi;

import java.security.SecureRandom;

public class Utils {

    private static final SecureRandom random = new SecureRandom();

    private static String digits = "0123456789abcdef";

    public static String toHex(byte[] data, int length) {
        StringBuffer buf = new StringBuffer();

        for (int i = 0; i != length; i++) {
            int v = data[i] & 0xff;

            buf.append(digits.charAt(v >> 4));
            buf.append(digits.charAt(v & 0xf));
        }

        return buf.toString();
    }

    public static byte[] generateRandomBytes(int length) {
        byte[] bytesArray = new byte[length];
        random.nextBytes(bytesArray);
        return bytesArray;
    }
}
