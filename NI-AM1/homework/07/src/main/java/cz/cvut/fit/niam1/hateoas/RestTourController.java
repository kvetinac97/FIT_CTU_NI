package cz.cvut.fit.niam1.hateoas;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.hateoas.CollectionModel;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.filter.ShallowEtagHeaderFilter;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.List;
import java.util.Locale;
import java.util.stream.Collectors;

import static org.springframework.hateoas.server.mvc.WebMvcLinkBuilder.*;

@RestController
@RequestMapping(value = "/tour")
public class RestTourController {

    @Autowired
    RestApplicationRepository repository;
    private final DateFormat sdf = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.ENGLISH);

    @GetMapping("/")
    public ResponseEntity<CollectionModel<Tour>> getTours(
        @RequestHeader(required = false, name = "If-Modified-Since") String lastModifiedReq
    ) {
        long lastModified;
        try { lastModified = sdf.parse(lastModifiedReq).getTime(); }
        catch (Exception exception) { lastModified = 0; }

        if (repository.getLastModified() <= lastModified)
            return ResponseEntity.status(HttpStatus.NOT_MODIFIED).build();

        List<Tour> tours = repository.getTours();
        CollectionModel<Tour> response = CollectionModel.of(tours);
        response.add(linkTo(methodOn(RestTourController.class).getTours(null)).withSelfRel());
        return ResponseEntity.ok()
                .lastModified(repository.getLastModified())
                .body(response);
    }

    @GetMapping("/etag-weak")
    public ResponseEntity<CollectionModel<Tour>> getWeakETagTours(
        @RequestHeader(required = false, name = "If-None-Match") String headerTag
    ) {
        List<Tour> tours = repository.getTours();
        int hash = tours.stream().map(Tour::getWeakHash).collect(Collectors.toList()).hashCode();

        String eTag = "W/\"" + hash + "\"";
        if (eTag.equals(headerTag))
            return ResponseEntity.status(HttpStatus.NOT_MODIFIED).build();

        CollectionModel<Tour> response = CollectionModel.of(tours);
        response.add(linkTo(methodOn(RestTourController.class).getWeakETagTours(null)).withSelfRel());
        return ResponseEntity
                .ok()
                .eTag(eTag)
                .body(response);
    }

    @GetMapping("/etag-strong")
    public CollectionModel<Tour> getStrongETagTours() {
        List<Tour> tours = repository.getTours();
        CollectionModel<Tour> response = CollectionModel.of(tours);
        response.add(linkTo(methodOn(RestTourController.class).getStrongETagTours()).withSelfRel());
        return response;
    }

    @GetMapping("/{id}")
    public HttpEntity<Tour> getTour(@PathVariable String id) {
        Tour tour = repository.getTourById(id);
        if (tour == null) return ResponseEntity.status(HttpStatus.NOT_FOUND).body(null);

        tour.removeLinks();
        tour.add(linkTo(RestTourController.class).slash(id).withSelfRel());
        tour.add(linkTo(methodOn(RestTourController.class).deleteTour(id)).withRel("remove"));
        tour.add(linkTo(methodOn(RestTourController.class).getTours(null)).withRel("list"));
        return ResponseEntity.status(HttpStatus.OK).body(tour);
    }

    @PostMapping(value = "/")
    public ResponseEntity createTour(@RequestBody Tour tour) {
        repository.addTour(tour);
        return ResponseEntity.status(HttpStatus.CREATED)
                .header("Location", "/tour/" + tour.getId())
                .build();
    }

    @PostMapping("/{id}/customer")
    public ResponseEntity createTourCustomer(@PathVariable String id, @RequestBody Customer customer) {
        repository.addTourCustomer(id, customer);
        return ResponseEntity.status(HttpStatus.CREATED)
                .header("Location", "/tour/" + id)
                .build();
    }

    @DeleteMapping("/{id}")
    public ResponseEntity deleteTour(@PathVariable String id) {
        repository.deleteTour(id);
        return ResponseEntity.status(HttpStatus.NO_CONTENT).build();
    }

    @Bean
    public FilterRegistrationBean<ShallowEtagHeaderFilter> shallowEtagHeaderFilter() {
        FilterRegistrationBean<ShallowEtagHeaderFilter> filterRegistrationBean = new FilterRegistrationBean<>(
                new ShallowEtagHeaderFilter()
        );
        filterRegistrationBean.addUrlPatterns("/tour/etag-strong");
        filterRegistrationBean.setName("etagFilter");
        return filterRegistrationBean;
    }

}
