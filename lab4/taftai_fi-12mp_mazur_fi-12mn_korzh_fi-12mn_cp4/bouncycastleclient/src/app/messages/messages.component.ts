import {Component, OnDestroy, OnInit} from '@angular/core';
import {Subscription} from "rxjs";
import {MessageService} from "../service/message.service";
import {Message} from "../model/message";

@Component({
  selector: 'app-messages',
  templateUrl: './messages.component.html',
  styleUrls: ['./messages.component.css']
})
export class MessagesComponent implements OnInit, OnDestroy {

  subscriptions: Subscription = new Subscription();

  messages: Message[] = [];


  constructor(private messageService: MessageService) {
  }

  ngOnInit(): void {
    this.getMessages();
  }


  getMessages(){
    this.messageService.getMessages().subscribe(data => {
      this.messages = data;
    });
  }
  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }

}
