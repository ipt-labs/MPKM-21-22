package ua.kpi.bouncycastle.entity;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class CreateUserResponseDto {
    private String username;
    private String privateKey;
    private String publicKey;
}
