package cz.cvut.fit.niam1.wsserver;

import https.courses_fit_cvut_cz.ni_am1.tutorials.web_services.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;

/*
 * vlastní logika
 */

@Endpoint
public class WebServiceEndpoint {

    @Autowired
    private WebServiceRepository repository;

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/tutorials/web-services/", localPart = "getToursRequest")
    @ResponsePayload
    public GetToursResponse getTours(@RequestPayload GetToursRequest request) {
        GetToursResponse response = new GetToursResponse();
        response.getTour().addAll(repository.getTours());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/tutorials/web-services/", localPart = "addToursRequest")
    @ResponsePayload
    public AddToursResponse addTours(@RequestPayload AddToursRequest request) {
        AddToursResponse response = new AddToursResponse();
        repository.addTour(request.getTour());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/tutorials/web-services/", localPart = "deleteToursRequest")
    @ResponsePayload
    public DeleteToursResponse addTours(@RequestPayload DeleteToursRequest request) {
        DeleteToursResponse response = new DeleteToursResponse();
        repository.deleteTour(request.getName());
        return response;
    }
}
