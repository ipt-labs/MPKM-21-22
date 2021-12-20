package ua.kpi.bouncycastle.entity;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class DecryptedMessage {
    private String message;
}
