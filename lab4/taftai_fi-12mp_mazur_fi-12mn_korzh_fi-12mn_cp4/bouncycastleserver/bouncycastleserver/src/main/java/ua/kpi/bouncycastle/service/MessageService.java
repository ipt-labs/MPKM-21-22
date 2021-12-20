package ua.kpi.bouncycastle.service;

import org.springframework.stereotype.Service;
import ua.kpi.bouncycastle.entity.CreateMessageDto;
import ua.kpi.bouncycastle.entity.DecryptedMessage;
import ua.kpi.bouncycastle.entity.Message;

import java.nio.charset.StandardCharsets;
import java.sql.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@Service
public class MessageService {

    private final List<Message> messages = new ArrayList<>();
    private final UserService userService;
    private final KeysService keysService;
    private final ElGamalService elGamalService;

    public MessageService(UserService userService,
                          KeysService keysService,
                          ElGamalService elGamalService) {
        this.userService = userService;
        this.keysService = keysService;
        this.elGamalService = elGamalService;
        messages.add(Message.builder()
                .id(UUID.randomUUID().toString())
                .createDate(new Date(System.currentTimeMillis()))
                .byteMessage(null)
                .message("hello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello wordhello word")
                .receiver("receiver")
                .sender("sender").build());
    }


    public Message createMessage(CreateMessageDto createMessageDto) throws Exception {
        var key = keysService.getPublicKeyByUser(createMessageDto.getReceiver());
        var ct = elGamalService.enc(createMessageDto.getPlainText().getBytes(StandardCharsets.UTF_8), key);
        var msg =  Message.builder()
                .id(UUID.randomUUID().toString())
                .createDate(new Date(System.currentTimeMillis()))
                .byteMessage(ct)
                .message(new String(ct, StandardCharsets.UTF_8))
                .receiver(createMessageDto.getReceiver())
                .sender(createMessageDto.getSender()).build();
        messages.add(msg);
        return msg;
    }

    public List<Message> getMessages() {
        return messages;
    }

    public Message getMessageById(String id) {
        return messages.stream().filter(msg -> msg.getId().equals(id)).findFirst().orElse(null);
    }

    public List<Message> getMessagesByReceiver(String username) {
        return messages.stream().filter(msg -> msg.getReceiver().equals(username)).collect(Collectors.toList());
    }

    public List<Message> getMessagesBySender(String username) {
        return messages.stream().filter(msg -> msg.getSender().equals(username)).collect(Collectors.toList());
    }

    public DecryptedMessage decrypt(String privateKey, String messageId) throws Exception {
        var msg = getMessageById(messageId);
        var key = keysService.findPrivateKey(privateKey).orElseThrow(() -> new RuntimeException("Incorrect private key"));
        return DecryptedMessage.builder().message(new String(elGamalService.dec(msg.getByteMessage(), key), StandardCharsets.UTF_8)).build();
    }

}
