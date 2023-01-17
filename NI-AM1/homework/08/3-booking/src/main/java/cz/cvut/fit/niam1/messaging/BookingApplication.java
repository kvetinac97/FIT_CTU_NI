package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Booking;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.jms.annotation.EnableJms;
import org.springframework.jms.annotation.JmsListener;

@SpringBootApplication
@EnableJms
public class BookingApplication {

    private final Logger logger = LoggerFactory.getLogger(BookingApplication.class);
    private final BookingRepository bookingRepository;

    public BookingApplication(BookingRepository bookingRepository) {
        this.bookingRepository = bookingRepository;
    }

    @JmsListener(destination = "bookingQueue")
    public void readBookingRequest(Booking booking) {
        logger.info("Accepting booking request for: {}", booking.getId());
        bookingRepository.addBooking(booking);
    }

    public static void main(String[] args) {
        SpringApplication.run(BookingApplication.class, args);
    }

}
