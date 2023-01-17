package cz.cvut.fit.niam1.messaging;

import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Info;
import io.swagger.v3.oas.models.info.License;
import org.apache.activemq.command.ActiveMQQueue;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.jms.annotation.EnableJms;
import org.springframework.jms.core.JmsTemplate;

import javax.jms.Queue;
import java.time.LocalDateTime;
import java.util.Objects;

@SpringBootApplication
@EnableJms
public class MessageProducerApplication {

    @Bean
    public Queue bookingQueue() {
        return new ActiveMQQueue("bookingQueue");
    }

    @Bean
    public Queue confirmationQueue() {
        return new ActiveMQQueue("confirmationQueue");
    }

    public static void main(String[] args) {
        SpringApplication.run(MessageProducerApplication.class, args);
    }

    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
            .info(new Info()
            .title("Rest Messaging Application")
            .version("1.0")
            .description("Rest Messaging Application")
            .termsOfService("http://swagger.io/terms/")
            .license(new License().name("Apache 2.0").url("http://springdoc.org")));
    }

}
