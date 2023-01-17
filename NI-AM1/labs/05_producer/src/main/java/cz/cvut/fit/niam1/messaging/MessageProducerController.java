package cz.cvut.fit.niam1.messaging;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.jms.annotation.JmsListener;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.web.bind.annotation.*;

import javax.jms.JMSException;
import javax.jms.Queue;

@RestController
@RequestMapping(value = "/messages")
public class MessageProducerController {

    private final Logger logger = LoggerFactory.getLogger(MessageProducerApplication.class);

    @Autowired
    private JmsTemplate jmsTemplate;

    @Autowired
    private Queue bookingQueue;

    @Autowired
    private Queue confirmationQueue;

    @Autowired
    MessageRepository messageRepository;

    @PostMapping(value = "/")
    public ResponseEntity sendMessage(@RequestBody Booking booking) throws JMSException {
        logger.info("Sending booking {} request to {}", booking.getId(), bookingQueue.getQueueName());
        jmsTemplate.convertAndSend(bookingQueue, booking);
        messageRepository.addBooking(booking);
        return ResponseEntity.status(HttpStatus.CREATED)
                .build();
    }

    @GetMapping(value = "/")
    public ResponseEntity getUnconfirmedMessages() throws JMSException {
        return ResponseEntity.status(HttpStatus.OK)
                .body(messageRepository.getBookings());
    }

    @JmsListener(destination = "confirmationQueue")
    public void readConfirmation(Booking booking) throws InterruptedException {
        logger.info("Received confirmation for: {}", booking.getId());
        messageRepository.removeBooking(booking);
    }

}
