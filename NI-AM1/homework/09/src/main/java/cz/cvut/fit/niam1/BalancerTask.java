package cz.cvut.fit.niam1;

import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.client.RestTemplate;

import java.net.URI;
import java.util.TimerTask;

public class BalancerTask extends TimerTask {

    private final BalancerRepository balancerRepository;
    private final RestTemplate restTemplate;

    public BalancerTask(BalancerRepository balancerRepository, RestTemplate restTemplate) {
        this.balancerRepository = balancerRepository;
        this.restTemplate = restTemplate;
    }

    @Override
    public void run() {
        System.out.println("Check servers...");
        for (ServerInfo server : balancerRepository.getServers()) {
            try {
                ResponseEntity<String> responseEntity = restTemplate.exchange(
                        new URI(server.getUrl()), HttpMethod.GET, null, String.class
                );
                if (responseEntity.getStatusCode().isError())
                    throw new Exception("Status code: " + responseEntity.getStatusCode().name());
                System.out.println("Server " + server.getUrl() + " check passed.");
                server.setActive(true);
            } catch (Exception exception) {
                System.out.println("Server " + server.getUrl() + " check failed: " + exception.getMessage());
                server.setActive(false);
            }
        }
    }
}
