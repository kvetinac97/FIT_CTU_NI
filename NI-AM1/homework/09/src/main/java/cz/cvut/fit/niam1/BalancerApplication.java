package cz.cvut.fit.niam1;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.event.ApplicationReadyEvent;
import org.springframework.context.annotation.Bean;
import org.springframework.context.event.EventListener;
import org.springframework.web.client.RestTemplate;

import java.util.Timer;

@SpringBootApplication
public class BalancerApplication {

    @Autowired
    private RestTemplate restTemplate;

    @Autowired
    private BalancerRepository balancerRepository;

    public static void main(String[] args) {
        SpringApplication.run(BalancerApplication.class, args);
    }

    @EventListener(ApplicationReadyEvent.class)
    public void startTask() {
        new Timer().scheduleAtFixedRate(new BalancerTask(balancerRepository, restTemplate), 0, 5000);
    }

    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

}
