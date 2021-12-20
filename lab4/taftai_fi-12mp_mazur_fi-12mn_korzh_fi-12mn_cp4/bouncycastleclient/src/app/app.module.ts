import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { MenuComponent } from './menu/menu.component';
import { MessagesComponent } from './messages/messages.component';
import { CreateUserComponent } from './create-user/create-user.component';
import { CreateMessageComponent } from './create-message/create-message.component';
import { ViewMessageComponent } from './view-message/view-message.component';
import {AppRoutingModule} from "./app-routing.module";
import {HttpClientModule} from "@angular/common/http";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";
import {DatePipe} from "@angular/common";

@NgModule({
  declarations: [
    AppComponent,
    MenuComponent,
    MessagesComponent,
    CreateUserComponent,
    CreateMessageComponent,
    ViewMessageComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    ReactiveFormsModule,
  ],
  providers: [HttpClientModule, DatePipe],
  bootstrap: [AppComponent]
})
export class AppModule { }
