package cz.cvut.fit.niam1;

import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;

import javax.servlet.http.HttpServletRequest;
import java.net.URI;
import java.util.List;
import java.util.Random;

@RestController
public class BalancerController {

    private final BalancerRepository balancerRepository;
    private final RestTemplate restTemplate;

    public BalancerController(BalancerRepository balancerRepository, RestTemplate restTemplate) {
        this.balancerRepository = balancerRepository;
        this.restTemplate = restTemplate;
    }

    @GetMapping(value = "/test")
    public ResponseEntity<String> test(HttpServletRequest request) throws Exception {
        // Select running server
        List<ServerInfo> servers = balancerRepository.getRunningServers().isEmpty() ?
                balancerRepository.getServers() : balancerRepository.getRunningServers();
        ServerInfo chosenServer = servers.get((new Random()).nextInt(servers.size()));
        if (!chosenServer.isActive())
            System.out.println("No running server found, trying anyways...");
        else
            System.out.println("Found running server: " + chosenServer.getUrl());

        HttpHeaders headers = new HttpHeaders();
        request.getHeaderNames().asIterator().forEachRemaining(head -> headers.add(head, request.getHeader(head)));
        HttpEntity<String> requestEntity = new HttpEntity<>(headers);
        ResponseEntity<String> responseEntity = restTemplate.exchange(new URI(chosenServer.getUrl()), HttpMethod.GET, requestEntity, String.class);
        System.out.println("Status code: " + responseEntity.getStatusCode().name());
        return responseEntity;
    }

}
