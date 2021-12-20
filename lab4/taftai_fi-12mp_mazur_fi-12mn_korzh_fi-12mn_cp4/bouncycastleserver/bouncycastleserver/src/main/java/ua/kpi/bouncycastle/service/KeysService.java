package ua.kpi.bouncycastle.service;

import org.springframework.stereotype.Service;
import ua.kpi.bouncycastle.entity.CreateUserResponseDto;

import java.nio.charset.StandardCharsets;
import java.security.*;
import java.util.*;

@Service
public class KeysService {

    private final Map<String, PrivateKey> usersPrivateKeys = new HashMap<>();
    private final Map<String, PublicKey> usersPublicKeys = new HashMap<>();

    private final UserService userService;


    private final SecureRandom random = new SecureRandom();

    private KeyPairGenerator kpGen;

    public KeysService(UserService userService) throws NoSuchProviderException, NoSuchAlgorithmException {
         kpGen = KeyPairGenerator.getInstance("ElGamal", "BC");
        this.userService = userService;
    }

    private KeyPair generateKeyPair(int keySize) {
        kpGen.initialize(keySize, random);
        return kpGen.generateKeyPair();
    }

    public void updateUsersPassword(String username, PrivateKey oldPassword, int keySize) {
        var users = userService.getUsers();
        if (oldPassword == null && !usersPrivateKeys.containsKey(username)) {
            if (!users.contains(username)) {
                throw new RuntimeException("User does not exist");
            }
        } else {
            if (usersPrivateKeys.containsKey(username) && oldPassword == null) {
                throw new RuntimeException("Old password must be specified");
            }
            if (usersPrivateKeys.containsKey(username) && oldPassword != null && !usersPrivateKeys.get(username).equals(oldPassword)) {
                throw new RuntimeException("Old password i incorrect be specified");
            }
        }

        var newKeyPair = generateKeyPair(keySize);
        usersPrivateKeys.put(username, newKeyPair.getPrivate());
        usersPublicKeys.put(username, newKeyPair.getPublic());
    }


    public CreateUserResponseDto createUser(String username, int keySize) {

        if (userService.getUsers().contains(username)) {
            throw new RuntimeException("User already exists");
        }

        userService.addUser(username);

        var newKeyPair = generateKeyPair(keySize);
        usersPrivateKeys.put(username, newKeyPair.getPrivate());
        usersPublicKeys.put(username, newKeyPair.getPublic());

        return CreateUserResponseDto.builder()
                .username(username)
                .privateKey(bytesToHex(newKeyPair.getPrivate().getEncoded()))
                .publicKey(bytesToHex(newKeyPair.getPublic().getEncoded())).build();
    }

    public Map<String, PublicKey> getPublicKeys() {
        return usersPublicKeys;
    }

    public PublicKey getPublicKeyByUser(String username) {
        return usersPublicKeys.get(username);
    }

    public Optional<PrivateKey> findPrivateKey(String privateKey) {
        return usersPrivateKeys.values().stream().filter(key -> bytesToHex(key.getEncoded()).equals(privateKey)).findFirst();
    }

    private static final char[] HEX_ARRAY = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = HEX_ARRAY[v >>> 4];
            hexChars[j * 2 + 1] = HEX_ARRAY[v & 0x0F];
        }
        return new String(hexChars);
    }
}
