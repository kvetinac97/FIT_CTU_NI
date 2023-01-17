package cz.cvut.fit.wrzecond;

import cz.cvut.fit.niam1.webservices.client.Payment;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import javax.xml.datatype.DatatypeConfigurationException;
import java.util.ArrayList;
import java.util.List;

@Component
public class WebServiceRepository {

    private static final List<Payment> payments = new ArrayList<>();

    @PostConstruct
    public void initRepo() throws DatatypeConfigurationException {
        Payment payment = new Payment();
        payment.setCardNumber("1234-5678-9012-3456");
        payment.setCardOwner("John Cena");
        payment.setOrderId(1);
        payments.add(payment);
    }

    public void addPayment(Payment payment){
        payments.add(payment);
    }

    public List<Payment> getPayments(){
        return payments;
    }

}
