package cz.cvut.fit.niam1.hateoas;

import cz.cvut.fit.niam1.hateoas.custom.CountryLinks;
import cz.cvut.fit.niam1.hateoas.custom.CustomLink;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.hateoas.CollectionModel;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;

import static org.springframework.hateoas.server.mvc.WebMvcLinkBuilder.*;

@RestController
@RequestMapping(value = "/country")
public class RestCountryController {

    @Autowired
    RestApplicationRepository repository;

    @GetMapping("/")
    public CollectionModel<Country> getCountries(@RequestHeader(required = false, name = "x-authorization") String auth) {
        List<Country> countries = repository.getCountries().stream().map(c -> {
            c.removeLinks();
            if (auth != null) // tady by mohla být pokročilejší kontrola..
                c.add(linkTo(methodOn(RestCountryController.class).getCountry(c.getId())).withRel("detail"));
            return c;
        }).collect(Collectors.toList());

        CollectionModel<Country> response = CollectionModel.of(countries);
        response.add(linkTo(methodOn(RestCountryController.class).getCountries("")).withSelfRel());

        // Někdy chci zpřístupnit operaci pouze tehdy, pokud k ní má klient práva
        if (auth != null) // tady by mohla být pokročilejší kontrola..
            response.add(linkTo(methodOn(RestCountryController.class).createCountry(null)).withRel("add"));
        return response;
    }

    @GetMapping("/{id}")
    public HttpEntity<Country> getCountry(@PathVariable String id) {
        Country country = repository.getCountryById(id);
        country.removeLinks(); // kvůli statickému repozitáři
        // country.add(linkTo(RestCountryController.class).slash(id).withSelfRel()); nebo
        country.add(linkTo(methodOn(RestCountryController.class).getCountry(id)).withSelfRel()); // celý kontroller
        country.add(linkTo(methodOn(RestCountryController.class).deleteCountry(id)).withRel("remove"));
        country.add(linkTo(methodOn(RestCountryController.class).getCountries("")).withRel("list"));
        return ResponseEntity.status(HttpStatus.OK).body(country);
    }

    @GetMapping("/c/{id}")
    public ResponseEntity getCustomCountry(@PathVariable String id) {
        Country country = repository.getCountryById(id);
        CustomLink[] links = new CustomLink[] {
                new CustomLink("http://127.0.0.1:8080/country/"+id, "self"),
                new CustomLink("http://127.0.0.1:8080/country/", "list"),
                new CustomLink("http://127.0.0.1:8080/country/"+id, "remove")
        };
        CountryLinks cl = new CountryLinks(country, links);
        return ResponseEntity.status(HttpStatus.OK).body(cl);
    }

    @PostMapping(value = "/")
    public ResponseEntity createCountry(@RequestBody Country country) {
        repository.addCountry(country);
        return ResponseEntity.status(HttpStatus.CREATED)
                .header("Location", "/country/" + country.getId())
                .build();
    }

    @DeleteMapping("/{id}")
    public ResponseEntity deleteCountry(@PathVariable String id) {
        repository.deleteCountry(id);
        return ResponseEntity.status(HttpStatus.NO_CONTENT)
                .build();
    }

}
