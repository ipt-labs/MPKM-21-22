import { Injectable } from '@angular/core';
import {environment} from "../../environments/environment";
import {HttpClient} from "@angular/common/http";
import {Observable} from "rxjs";
import {Message} from "../model/message";
import {CreateMessageDto} from "../model/create-message-dto";
import {DecryptedMessage} from "../model/decrypted-message";

@Injectable({
  providedIn: 'root'
})
export class MessageService {

  private url = `${environment.url}`;
  private messageUrl = this.url + 'message';

  constructor(private http: HttpClient) {
  }

  getMessages(): Observable<Message[]> {
    return this.http.get<Message[]>(this.messageUrl);
  }

  getMessage(id: string): Observable<Message> {
    return this.http.get<Message>(`${this.messageUrl}/${id}`);
  }

  createMessage(dto: CreateMessageDto): Observable<Message> {
    return this.http.post<Message>(this.messageUrl, dto);
  }

  decryptMessage(id:string, dto: string): Observable<DecryptedMessage> {
    return this.http.post<DecryptedMessage>(`${this.messageUrl}/${id}/decrypt`, dto);
  }

/*

 @PostMapping
    public Message createMessages(@RequestBody CreateMessageDto createMessageDto) throws Exception {
        return messageService.createMessage(createMessageDto);
    }

    @GetMapping("/{id}")
    public Message getMessage(@PathVariable String id) {
        return messageService.getMessageById(id);
    }

    @PostMapping("/{id}/decrypt")
    public String decrypt(@RequestBody  String privateKey, @PathVariable String id) throws Exception {
        return messageService.decrypt(privateKey, id);
    }


  getAdminBoard(): Observable<string> {
    return this.http.get(this.adminUrl, {responseType: 'text'});
  }

  getUserById(id: number): Observable<User> {
    const url = `${this.url}users/${id}`;
    return this.http.get<User>(url);
    // return this.http.get<User>(`${this.url}/users/${id}`);
  }

  updateUser(user: User) {
    const url = `${this.url}users/update`;
    const body = {id: user.id, email: user.email, name: user.name, surname: user.surname, role: user.role, enabled: user.enabled};
    return this.http.put(url, body);
  }
*/

}
