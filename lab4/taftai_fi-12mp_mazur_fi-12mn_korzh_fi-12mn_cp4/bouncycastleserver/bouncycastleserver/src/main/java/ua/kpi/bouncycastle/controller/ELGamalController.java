package ua.kpi.bouncycastle.controller;

import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Slf4j
@RequestMapping("/api")
public class ELGamalController {

    @GetMapping("/hello")
    public String hello(){
        log.info("he;lool here ");
        return "hello worllff";
    }
}
