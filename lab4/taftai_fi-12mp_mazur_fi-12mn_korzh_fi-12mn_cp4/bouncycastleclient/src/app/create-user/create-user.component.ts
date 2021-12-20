import {Component, OnInit} from '@angular/core';
import {Subscription} from "rxjs";
import {FormBuilder, FormControl, Validators} from "@angular/forms";
import {UserService} from "../service/user.service";

@Component({
  selector: 'app-create-user',
  templateUrl: './create-user.component.html',
  styleUrls: ['./create-user.component.css']
})
export class CreateUserComponent implements OnInit {


  subscriptions: Subscription = new Subscription();


  username = '';
  userForm: any;
  privateKey = '';
  publicKey = '';

  constructor(private formBuilder: FormBuilder, private userService: UserService) {
    this.userForm = this.formBuilder.group({
      name: new FormControl(this.username, [Validators.required])
    });
  }

  submit() {
    if (!this.userForm.invalid) {
      this.userService.createUser(this.username).subscribe(data => {
        this.privateKey = data.privateKey;
        this.publicKey = data.publicKey;
      })
    }
  }

  ngOnInit(): void {
  }

}
