package cz.cvut.fit.niam1.messaging;

import org.apache.activemq.command.ActiveMQQueue;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.jms.annotation.EnableJms;
import org.springframework.jms.annotation.JmsListener;
import org.springframework.jms.core.JmsTemplate;

import javax.jms.JMSException;
import javax.jms.Queue;

@SpringBootApplication
@EnableJms
public class MessageConsumerApplication {

    @Autowired
    private JmsTemplate jmsTemplate;

    @Autowired
    private Queue confirmationQueue;

    private final Logger logger = LoggerFactory.getLogger(MessageConsumerApplication.class);

    @JmsListener(destination = "bookingQueue")
    public void readBookingRequest(Booking booking) throws InterruptedException, JMSException {
        logger.info("Received booking request for: {}", booking.getId());
        Thread.sleep(5000);
        logger.info("Sending confirmation for: {} on {}", booking.getId(), confirmationQueue.getQueueName());
        jmsTemplate.convertAndSend(confirmationQueue, booking);
    }

    @Bean
    public Queue confirmationQueue() {
        return new ActiveMQQueue("confirmationQueue");
    }

    public static void main(String[] args) {
        SpringApplication.run(MessageConsumerApplication.class, args);
    }

}
