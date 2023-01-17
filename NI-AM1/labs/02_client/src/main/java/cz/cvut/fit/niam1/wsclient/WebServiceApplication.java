package cz.cvut.fit.niam1.wsclient;

import https.courses_fit_cvut_cz.ni_am1.tutorials.web_services.*;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;

import java.util.stream.Collectors;

@SpringBootApplication
public class WebServiceApplication {

    public static void main(String[] args) {
        SpringApplication.run(WebServiceApplication.class, args);
    }

    @Bean
    CommandLineRunner lookup(WebServiceClient wsClient) {
        return args -> {
            GetToursResponse response = wsClient.getTours();
            System.out.println(response.getTour().stream().map( tour -> tour.getName()+"("+tour.getType()+")" ).collect( Collectors.joining( "; " ) ));

            Tour t = new Tour();
            t.setName("Client Test Tour");
            t.setType(TourType.CRUISES);
            AddToursResponse response2 = wsClient.addTour(t);
            System.out.println("Should be OK");
        };
    }
}