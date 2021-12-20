package ua.kpi.bouncycastle.controller;

import org.springframework.web.bind.annotation.*;
import ua.kpi.bouncycastle.entity.CreateUserResponseDto;
import ua.kpi.bouncycastle.service.KeysService;
import ua.kpi.bouncycastle.service.UserService;

import java.util.List;


@RestController
@RequestMapping("/user")
@CrossOrigin("http://localhost:4200")
public class UserController {

    private final UserService userService;
    private final KeysService keysService;

    private final int keySize = 2048 / 8;
    public UserController(UserService userService, KeysService keysService) {
        this.userService = userService;
        this.keysService = keysService;
    }


    @GetMapping
    public List<String> getUsers(){
        return userService.getUsers();
    }


    @PostMapping
    public CreateUserResponseDto createUser(@RequestBody String username){
        return keysService.createUser(username, keySize);
    }

}
