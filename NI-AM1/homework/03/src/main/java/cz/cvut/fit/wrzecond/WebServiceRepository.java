package cz.cvut.fit.wrzecond;

import https.courses_fit_cvut_cz.ni_am1.homeworks.bookings.AirportDateTime;
import https.courses_fit_cvut_cz.ni_am1.homeworks.bookings.Booking;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.List;

@Component
public class WebServiceRepository {

    private static final List<Booking> bookings = new ArrayList<>();

    @PostConstruct
    public void initRepo() throws DatatypeConfigurationException {
        Booking vacation = new Booking();

        AirportDateTime departure = new AirportDateTime();
        departure.setAirport("Miami International Airport (MIA)");
        GregorianCalendar c = new GregorianCalendar(2022, Calendar.JULY, 12, 12, 20);
        XMLGregorianCalendar departureDate = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);
        departure.setDateTime(departureDate);

        AirportDateTime arrival = new AirportDateTime();
        arrival.setAirport("Prague Airport (PRG)");
        GregorianCalendar cal = new GregorianCalendar(2022, Calendar.JULY, 12, 14, 20);
        XMLGregorianCalendar arrivalDate = DatatypeFactory.newInstance().newXMLGregorianCalendar(cal);
        departure.setDateTime(arrivalDate);

        vacation.setId(1);
        vacation.setDeparture(departure);
        vacation.setArrival(arrival);
        bookings.add(vacation);
    }

    public void addBooking(Booking booking){
        bookings.add(booking);
    }

    public List<Booking> getBookings(){
        return bookings;
    }

    public void deleteBooking(int id){
        bookings.removeIf(t -> t.getId() == id);
    }

    public void updateBooking(Booking booking) {
        int index = -1;
        for (int i = 0; i < bookings.size(); ++i)
            if (bookings.get(i).getId() == booking.getId()) {
                index = i;
                break;
            }

        if (index == -1) return;
        bookings.set(index, booking);
    }

}
