package cz.cvut.fit.niam1;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.web.bind.annotation.*;

@RestController
@EnableAutoConfiguration
public class HelloWebApplication {

    @GetMapping("/hello/{name}")
    String helloName(@PathVariable String name) {
        return "Hello " + name;
    }

    @GetMapping("/")
    String homepage() { return "Default content"; }

    @PostMapping(value = "/hello", consumes = "text/plain")
    String helloMessage(@RequestBody String message) {
        return message;
    }

    @PostMapping(value = "/add/{number}", consumes = "text/plain")
    Integer addNumber(@PathVariable Integer number, @RequestBody String number2) { return number + Integer.parseInt(number2); }

    @PostMapping(value = "/transform", consumes = "application/xml")
    String transform(@RequestBody String message) {
        return message;
    }

    public static void main(String[] args) {
        SpringApplication.run(HelloWebApplication.class, args);
    }

}