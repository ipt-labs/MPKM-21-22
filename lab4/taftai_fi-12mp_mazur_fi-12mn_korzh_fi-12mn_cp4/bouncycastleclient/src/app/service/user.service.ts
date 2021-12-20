import { Injectable } from '@angular/core';
import {Observable} from "rxjs";
import {Message} from "../model/message";
import {CreateMessageDto} from "../model/create-message-dto";
import {CreateUserResponseDto} from "../model/create-user-response-dto";
import {environment} from "../../environments/environment";
import {HttpClient} from "@angular/common/http";

@Injectable({
  providedIn: 'root'
})
export class UserService {


  private url = `${environment.url}`;
  private userUrl = this.url + 'user';

  constructor(private http: HttpClient) {
  }

  getUsers(): Observable<Message[]> {
    return this.http.get<Message[]>(this.userUrl);
  }


  createUser(username: string): Observable<CreateUserResponseDto> {
    return this.http.post<CreateUserResponseDto>(this.userUrl, username);
  }
}

/*

    @GetMapping
    public List<String> getUsers(){
        return userService.getUsers();
    }


    @PostMapping
    public CreateUserResponseDto createUser(@RequestBody String username){
        return keysService.createUser(username, keySize);
    }*/
