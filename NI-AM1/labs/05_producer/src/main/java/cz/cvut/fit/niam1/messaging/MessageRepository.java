package cz.cvut.fit.niam1.messaging;

import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
public class MessageRepository {

    private static List<Booking> bookings;

    public MessageRepository() {
        bookings = new ArrayList<>();
    }

    public List<Booking> getBookings() {
        return bookings;
    }

    public void addBooking (Booking booking) {
        bookings.add(booking);
    }

    public void removeBooking (Booking booking) {
        bookings.removeIf(b -> b.getId().equals(booking.getId()));
    }
}
