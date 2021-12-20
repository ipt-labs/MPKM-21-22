import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import {RouterModule, Routes} from '@angular/router';
import {MenuComponent} from "./menu/menu.component";
import {MessagesComponent} from "./messages/messages.component";
import {CreateUserComponent} from "./create-user/create-user.component";
import {CreateMessageComponent} from "./create-message/create-message.component";
import {ViewMessageComponent} from "./view-message/view-message.component";



const routes: Routes = [
  { path: 'menu', component: MenuComponent},
  { path: 'create-user', component: CreateUserComponent},
  { path: 'create-message', component: CreateMessageComponent},
  { path: 'messages', component: MessagesComponent},
  { path: 'messages/:id', component: ViewMessageComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
