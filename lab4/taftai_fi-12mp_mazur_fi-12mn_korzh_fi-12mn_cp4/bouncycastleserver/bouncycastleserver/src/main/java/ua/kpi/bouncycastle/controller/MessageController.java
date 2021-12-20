package ua.kpi.bouncycastle.controller;

import org.springframework.web.bind.annotation.*;
import ua.kpi.bouncycastle.entity.CreateMessageDto;
import ua.kpi.bouncycastle.entity.DecryptedMessage;
import ua.kpi.bouncycastle.entity.Message;
import ua.kpi.bouncycastle.service.MessageService;

import java.util.List;

@RestController
@RequestMapping("/message")
@CrossOrigin("http://localhost:4200")
public class MessageController {

    public final MessageService messageService;
    public MessageController(MessageService messageService) {
        this.messageService = messageService;
    }


    @GetMapping
    public List<Message> getMessages(){
        return messageService.getMessages();
    }


    @PostMapping
    public Message createMessages(@RequestBody CreateMessageDto createMessageDto) throws Exception {
        return messageService.createMessage(createMessageDto);
    }

    @GetMapping("/{id}")
    public Message getMessage(@PathVariable String id) {
        return messageService.getMessageById(id);
    }

    @PostMapping("/{id}/decrypt")
    public DecryptedMessage decrypt(@RequestBody  String privateKey, @PathVariable String id) throws Exception {
        return messageService.decrypt(privateKey, id);
    }

}
