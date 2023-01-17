package cz.cvut.fit.wrzecond.client;

import cz.cvut.fit.niam1.webservices.client.ValidateCardRequest;
import cz.cvut.fit.niam1.webservices.client.ValidateCardResponse;
import org.springframework.ws.client.core.support.WebServiceGatewaySupport;

public class WebServiceClient extends WebServiceGatewaySupport {

    public ValidateCardResponse validateCard(String cardNumber, String cardOwner) {
        ValidateCardRequest request = new ValidateCardRequest();
        request.setCardNumber(cardNumber);
        request.setCardOwner(cardOwner);
        return  (ValidateCardResponse) getWebServiceTemplate()
                .marshalSendAndReceive(request);
    }

}
