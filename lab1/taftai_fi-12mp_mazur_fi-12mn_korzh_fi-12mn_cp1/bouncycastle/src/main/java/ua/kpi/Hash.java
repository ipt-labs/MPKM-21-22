package ua.kpi;

import java.security.MessageDigest;

public class Hash {

    public byte[] SHA256(byte[] input) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        md.update(input);
        return md.digest();
    }
}
