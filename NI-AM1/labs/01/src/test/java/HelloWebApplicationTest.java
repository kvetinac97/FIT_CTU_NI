import cz.cvut.fit.niam1.HelloWebApplication;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.boot.web.server.LocalServerPort;
import org.springframework.http.HttpEntity;
import org.springframework.http.ResponseEntity;

import java.net.URL;

import static org.assertj.core.api.Assertions.assertThat;

@SpringBootTest(classes = HelloWebApplication.class, webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class HelloWebApplicationTest {

    @LocalServerPort
    private int port;

    private URL base;

    @Autowired
    private TestRestTemplate template;

    @BeforeEach
    public void setUp() throws Exception {
        this.base = new URL("http://localhost:" + port + "/");
    }

    @Test
    public void getHello() throws Exception {
        String name = "world";
        ResponseEntity<String> response = template.getForEntity(base.toString()+"hello/"+name, String.class);
        assertThat(response.getBody()).isEqualTo("Hello "+name);
    }

    @Test
    public void testMath() throws Exception {
        int one = 1;
        HttpEntity<String> request = new HttpEntity<>(Integer.toString(one));
        ResponseEntity<String> response = template.postForEntity(base.toString()+"add/"+one, request, String.class);
        assertThat(response.getBody()).isEqualTo("2");
    }
}