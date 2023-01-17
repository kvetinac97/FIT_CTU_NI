package cz.cvut.fit.niam1.wsclient;

import https.courses_fit_cvut_cz.ni_am1.tutorials.web_services.*;
import org.springframework.ws.client.core.support.WebServiceGatewaySupport;
import org.springframework.ws.soap.client.core.SoapActionCallback;

public class WebServiceClient extends WebServiceGatewaySupport {

    public GetToursResponse getTours() {
        // Sestaví request a odešle ho
        GetToursRequest request = new GetToursRequest();
        return  (GetToursResponse) getWebServiceTemplate()
                .marshalSendAndReceive(request);
    }

    public AddToursResponse addTour(Tour tour) {
        AddToursRequest request = new AddToursRequest();
        request.setTour(tour);
        return  (AddToursResponse) getWebServiceTemplate()
                .marshalSendAndReceive(request);
    }

}
