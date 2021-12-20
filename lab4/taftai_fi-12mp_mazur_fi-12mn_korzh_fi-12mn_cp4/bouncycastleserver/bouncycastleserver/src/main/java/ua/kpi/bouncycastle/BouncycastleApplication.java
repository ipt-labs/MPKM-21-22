package ua.kpi.bouncycastle;

import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import javax.crypto.Cipher;
import java.security.NoSuchAlgorithmException;
import java.security.Security;

@SpringBootApplication
public class BouncycastleApplication {

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

	public static void main(String[] args) {
		SpringApplication.run(BouncycastleApplication.class, args);
	}

}
