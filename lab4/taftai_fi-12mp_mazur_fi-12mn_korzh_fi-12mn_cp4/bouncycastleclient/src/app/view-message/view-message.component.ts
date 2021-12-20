import { Component, OnInit } from '@angular/core';
import {Subscription} from "rxjs";
import {Message} from "../model/message";
import {ActivatedRoute} from "@angular/router";
import {MessageService} from "../service/message.service";
import {DatePipe} from "@angular/common";

@Component({
  selector: 'app-view-message',
  templateUrl: './view-message.component.html',
  styleUrls: ['./view-message.component.css'],
  providers: [DatePipe]
})
export class ViewMessageComponent implements OnInit {


  subscriptions: Subscription = new Subscription();

  message: Message | undefined;
  privateKey = '';
  id = '';
  decrypted = '';

  constructor(private route: ActivatedRoute, private messageService: MessageService) {
  }

  ngOnInit(): void {
    this.subscriptions.add(this.route.paramMap.subscribe(value => {
      const id = value.get('id');
      if (id) {
        this.messageService.getMessage(id).subscribe(data => {
          this.message = data;
          this.id = id;
        });
      }
    }));
  }
  decode() {
    this.subscriptions.add(this.messageService.decryptMessage(this.id, this.privateKey).subscribe(data =>{
      this.decrypted = data.message;
    }))
  }
}
