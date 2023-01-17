package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Order;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.web.bind.annotation.*;

import javax.jms.JMSException;
import javax.jms.Queue;

@RestController
public class AcceptorController {

    private final Logger logger = LoggerFactory.getLogger(AcceptorApplication.class);
    private final JmsTemplate jmsTemplate;
    private final Queue orderQueue;

    public AcceptorController(JmsTemplate jmsTemplate, Queue orderQueue) {
        this.jmsTemplate = jmsTemplate;
        this.orderQueue = orderQueue;
    }

    @PostMapping(value = "/order")
    @ResponseStatus(HttpStatus.CREATED)
    public void orderRequest(@RequestBody Order order) throws JMSException {
        logger.info("Sending order {} request to {}", order.getId(), orderQueue.getQueueName());
        jmsTemplate.convertAndSend(orderQueue, order);
    }

}
