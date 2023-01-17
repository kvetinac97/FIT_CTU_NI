package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Trip;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
public class TripController {

    private final TripRepository tripRepository;

    public TripController(TripRepository tripRepository) {
        this.tripRepository = tripRepository;
    }

    @GetMapping(value = "/trip")
    public List<Trip> getTrips() {
        return tripRepository.getTrips();
    }

}
