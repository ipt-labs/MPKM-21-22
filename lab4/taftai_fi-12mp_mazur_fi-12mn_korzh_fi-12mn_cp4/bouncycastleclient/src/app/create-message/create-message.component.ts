import { Component, OnInit } from '@angular/core';
import {Subscription} from "rxjs";
import {FormBuilder, FormControl, Validators} from "@angular/forms";
import {UserService} from "../service/user.service";
import {MessageService} from "../service/message.service";
import {Message} from "../model/message";
import {CreateMessageDto} from "../model/create-message-dto";

@Component({
  selector: 'app-create-message',
  templateUrl: './create-message.component.html',
  styleUrls: ['./create-message.component.css']
})
export class CreateMessageComponent  implements OnInit {

  subscriptions: Subscription = new Subscription();


  messageDto: CreateMessageDto = {receiver: '', sender: '', plainText: ''};
  message: Message | undefined;
  messageForm: any;

  constructor(private formBuilder: FormBuilder, private messageService: MessageService) {
    this.messageForm = this.formBuilder.group({
      sender: new FormControl(this.messageDto.sender, [Validators.required]),
      receiver: new FormControl(this.messageDto.receiver, [Validators.required]),
      plainText: new FormControl(this.messageDto.plainText, [Validators.required]),
    });
  }

  submit() {
    if (!this.messageForm.invalid) {
      this.messageService.createMessage(this.messageDto).subscribe(data => {
        this.message = data;
      })
    }
  }

  ngOnInit(): void {
  }

}
