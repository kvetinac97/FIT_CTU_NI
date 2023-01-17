package cz.cvut.fit.wrzecond;

import https.courses_fit_cvut_cz.ni_am1.homeworks.bookings.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;

@Endpoint
public class WebServiceEndpoint {

    @Autowired
    private WebServiceRepository repository;

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/bookings/", localPart = "getBookingRequest")
    @ResponsePayload
    public GetBookingResponse getBookings(@RequestPayload GetBookingRequest request) {
        GetBookingResponse response = new GetBookingResponse();
        response.getBooking().addAll(repository.getBookings());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/bookings/", localPart = "addBookingRequest")
    @ResponsePayload
    public AddBookingResponse addBookings(@RequestPayload AddBookingRequest request) {
        AddBookingResponse response = new AddBookingResponse();
        repository.addBooking(request.getBooking());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/bookings/", localPart = "updateBookingRequest")
    @ResponsePayload
    public UpdateBookingResponse addBookings(@RequestPayload UpdateBookingRequest request) {
        UpdateBookingResponse response = new UpdateBookingResponse();
        repository.updateBooking(request.getBooking());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/bookings/", localPart = "deleteBookingRequest")
    @ResponsePayload
    public DeleteBookingResponse addBookings(@RequestPayload DeleteBookingRequest request) {
        DeleteBookingResponse response = new DeleteBookingResponse();
        repository.deleteBooking(request.getId());
        return response;
    }
}
