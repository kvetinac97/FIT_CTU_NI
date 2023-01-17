package cz.cvut.fit.niam1.messaging;

import org.apache.activemq.broker.BrokerService;
import org.apache.activemq.broker.region.policy.PolicyEntry;
import org.apache.activemq.broker.region.policy.PolicyMap;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.ArrayList;
import java.util.List;

@Configuration
public class AcceptorConfig {

    @Bean
    public BrokerService broker(PolicyMap policyMap) throws Exception {
        BrokerService brokerService = new BrokerService();
        brokerService.addConnector("tcp://localhost:61616");
        brokerService.setPersistent(false);
        brokerService.setDestinationPolicy(policyMap); // TODO policyMap()
        return brokerService;
    }

    @Bean
    public PolicyMap policyMap(){
        PolicyMap destinationPolicy = new PolicyMap();
        List<PolicyEntry> entries = new ArrayList<>();
        PolicyEntry queueEntry = new PolicyEntry();
        queueEntry.setQueue(">");
        queueEntry.setStrictOrderDispatch(false);
        entries.add(queueEntry);
        destinationPolicy.setPolicyEntries(entries);
        return destinationPolicy;
    }

}
