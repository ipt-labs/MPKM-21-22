package ua.kpi.bouncycastle.entity;

import lombok.Builder;
import lombok.Data;

import java.sql.Date;

@Data
@Builder
public class Message {
    private String id;
    private Date createDate;
    private String receiver;
    private String sender;
    private String message;
    private byte[] byteMessage;
}
