package cz.cvut.fit.niam1.messaging;

import cz.cvut.fit.niam1.messaging.data.Booking;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
public class BookingController {

    private final BookingRepository bookingRepository;

    public BookingController(BookingRepository bookingRepository) {
        this.bookingRepository = bookingRepository;
    }

    @GetMapping(value = "/booking")
    public List<Booking> getBookings() {
        return bookingRepository.getBookings();
    }

}
