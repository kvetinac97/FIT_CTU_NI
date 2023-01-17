package cz.cvut.fit.niam1.messaging;

import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Info;
import io.swagger.v3.oas.models.info.License;
import org.apache.activemq.command.ActiveMQQueue;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.jms.annotation.EnableJms;

import javax.jms.Queue;

@SpringBootApplication
@EnableJms
public class AcceptorApplication {

    @Bean
    public Queue orderQueue() {
        return new ActiveMQQueue("orderQueue");
    }

    public static void main(String[] args) {
        SpringApplication.run(AcceptorApplication.class, args);
    }

    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
            .info(new Info()
                .title("Booking Messaging Application")
                .version("1.0")
                .description("Booking Messaging Application")
                .termsOfService("https://swagger.io/terms/")
                .license(new License().name("Apache 2.0").url("https://springdoc.org"))
            );
    }

}
