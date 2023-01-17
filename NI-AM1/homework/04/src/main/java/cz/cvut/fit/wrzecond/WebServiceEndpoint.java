package cz.cvut.fit.wrzecond;

import cz.cvut.fit.niam1.webservices.client.*;
import cz.cvut.fit.wrzecond.client.WebServiceClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;

@Endpoint
public class WebServiceEndpoint {

    @Autowired
    private WebServiceRepository repository;

    @Autowired
    private WebServiceClient client;

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/payments/", localPart = "getPaymentRequest")
    @ResponsePayload
    public GetPaymentResponse getPayments(@RequestPayload GetPaymentRequest request) {
        GetPaymentResponse response = new GetPaymentResponse();
        response.getPayment().addAll(repository.getPayments());
        return response;
    }

    @PayloadRoot(namespace = "https://courses.fit.cvut.cz/NI-AM1/homeworks/payments/", localPart = "addPaymentRequest")
    @ResponsePayload
    public AddPaymentResponse addPayments(@RequestPayload AddPaymentRequest request) {
        AddPaymentResponse response = new AddPaymentResponse();
        boolean validationResult = client.validateCard(
                request.getPayment().getCardNumber(),
                request.getPayment().getCardOwner()
        ).isResult();

        if (validationResult)
            repository.addPayment(request.getPayment());

        return response;
    }
}
