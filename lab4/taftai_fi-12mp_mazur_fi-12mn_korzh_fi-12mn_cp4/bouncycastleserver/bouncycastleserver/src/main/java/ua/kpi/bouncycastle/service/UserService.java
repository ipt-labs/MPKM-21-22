package ua.kpi.bouncycastle.service;

import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class UserService {

    private final List<String> users = new ArrayList<>();

    public void addUser(String username) {
        users.add(username);
    }
    
    public List<String> getUsers() {
        return users;
    }
}
