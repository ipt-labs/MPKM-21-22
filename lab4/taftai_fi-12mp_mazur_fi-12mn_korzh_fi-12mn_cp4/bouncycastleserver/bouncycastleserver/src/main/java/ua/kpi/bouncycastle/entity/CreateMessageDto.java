package ua.kpi.bouncycastle.entity;

import lombok.Data;

@Data
public class CreateMessageDto {
    private String receiver;
    private String sender;
    private String plainText;
}
