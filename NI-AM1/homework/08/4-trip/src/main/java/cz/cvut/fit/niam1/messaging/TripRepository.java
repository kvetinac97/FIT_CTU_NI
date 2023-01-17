package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Trip;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
public class TripRepository {

    private static final List<Trip> trips = new ArrayList<>();

    public List<Trip> getTrips() {
        return trips;
    }

    public void addTrip (Trip trip) {
        trips.add(trip);
    }

}
