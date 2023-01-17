package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Trip;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.jms.annotation.EnableJms;
import org.springframework.jms.annotation.JmsListener;

@SpringBootApplication
@EnableJms
public class TripApplication {

    private final Logger logger = LoggerFactory.getLogger(TripApplication.class);
    private final TripRepository tripRepository;

    public TripApplication(TripRepository tripRepository) {
        this.tripRepository = tripRepository;
    }

    @JmsListener(destination = "tripQueue")
    public void readBookingRequest(Trip trip) {
        logger.info("Accepting trip request for: {}", trip.getId());
        tripRepository.addTrip(trip);
    }

    public static void main(String[] args) {
        SpringApplication.run(TripApplication.class, args);
    }

}
