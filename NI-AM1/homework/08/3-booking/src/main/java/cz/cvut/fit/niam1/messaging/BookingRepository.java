package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Booking;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
public class BookingRepository {

    private static List<Booking> bookings = new ArrayList<>();

    public List<Booking> getBookings() {
        return bookings;
    }

    public void addBooking (Booking booking) {
        bookings.add(booking);
    }

}
