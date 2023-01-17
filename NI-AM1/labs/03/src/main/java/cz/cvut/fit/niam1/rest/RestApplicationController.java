package cz.cvut.fit.niam1.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;


@RestController
public class RestApplicationController {

    @Autowired
    RestApplicationRepository repository;

    @GetMapping("/country")
    @ResponseStatus(HttpStatus.OK)
    public List<Country> getCountries() {
        return repository.getCountries();
    }

    @GetMapping("/country/{id}")
    @ResponseStatus(HttpStatus.OK)
    public ResponseEntity<Country> getCountry(@PathVariable String id) {
        Country c = repository.getCountryById(id);
        return ResponseEntity.status(c == null ? HttpStatus.NOT_FOUND : HttpStatus.OK).body(c);
    }

    @PostMapping(value = "/country")
    public ResponseEntity createCountry(@RequestBody Country country) {
        repository.addCountry(country);
        return ResponseEntity.status(HttpStatus.CREATED).build();
    }

    @PutMapping("/country/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void updateCountry(@PathVariable String id, @RequestBody Country country) {
        repository.updateCountry(id, country);
    }

    @DeleteMapping("/country/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public ResponseEntity deleteCountry(@PathVariable String id) {
        Country c = repository.getCountryById(id);
        repository.deleteCountry(id);
        return ResponseEntity.status(c == null ? HttpStatus.NOT_FOUND : HttpStatus.NO_CONTENT).build();
    }

    @GetMapping("/tour")
    @ResponseStatus(HttpStatus.OK)
    public List<Tour> getTours() {
        return repository.getTours();
    }

    @GetMapping("/tour/{id}")
    @ResponseStatus(HttpStatus.OK)
    public ResponseEntity<Tour> getTour(@PathVariable String id) {
        Tour t = repository.getTourById(id);
        return ResponseEntity.status(t == null ? HttpStatus.NOT_FOUND : HttpStatus.OK).body(t);
    }

    @PutMapping("/tour/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void updateTour(@PathVariable String id, @RequestBody Tour tour) {
        repository.updateTour(id, tour);
    }

    @DeleteMapping("/tour/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public ResponseEntity deleteTour(@PathVariable String id) {
        Tour t = repository.getTourById(id);
        repository.deleteTour(id);
        return ResponseEntity.status(t == null ? HttpStatus.NOT_FOUND : HttpStatus.NO_CONTENT).build();
    }

    @PostMapping(value = "/tour")
    public ResponseEntity createTour(@RequestBody Tour tour) {
        repository.addTour(tour);
        return ResponseEntity
                .status(HttpStatus.CREATED)
                .header("Location", "/tour/" + tour.getId())
                .build();
    }
}
