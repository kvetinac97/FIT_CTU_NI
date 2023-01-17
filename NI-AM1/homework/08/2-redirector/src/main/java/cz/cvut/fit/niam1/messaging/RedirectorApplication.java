package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Booking;
import cz.cvut.fit.niam1.messaging.data.Order;
import cz.cvut.fit.niam1.messaging.data.Trip;
import org.apache.activemq.command.ActiveMQQueue;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.jms.annotation.EnableJms;
import org.springframework.jms.annotation.JmsListener;
import org.springframework.jms.core.JmsTemplate;

import javax.jms.Queue;

@SpringBootApplication
@EnableJms
public class RedirectorApplication {

    private final Logger logger = LoggerFactory.getLogger(RedirectorApplication.class);
    private final JmsTemplate jmsTemplate;

    public RedirectorApplication(JmsTemplate jmsTemplate) {
        this.jmsTemplate = jmsTemplate;
    }

    @JmsListener(destination = "orderQueue")
    public void readBookingRequest(Order order) throws Exception {
        logger.info("Received order request for: {}", order.getId());
        Queue queue = switch (order.getOrderType()) {
            case TRIP -> tripQueue();
            case BOOKING -> bookingQueue();
        };
        Object data = switch (order.getOrderType()) {
            case TRIP -> new Trip(order.getId(), order.getName());
            case BOOKING -> new Booking(order.getId(), order.getName());
        };
        logger.info("Redirecting order {} request to {}", order.getId(), queue.getQueueName());
        jmsTemplate.convertAndSend(queue, data);
    }

    @Bean
    public Queue bookingQueue() {
        return new ActiveMQQueue("bookingQueue");
    }

    @Bean
    public Queue tripQueue() {
        return new ActiveMQQueue("tripQueue");
    }

    public static void main(String[] args) {
        SpringApplication.run(RedirectorApplication.class, args);
    }

}
