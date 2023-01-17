package cz.cvut.fit.niam1.hateoas;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.hateoas.CollectionModel;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

import static org.springframework.hateoas.server.mvc.WebMvcLinkBuilder.*;

@RestController
@RequestMapping(value = "/tour")
public class RestTourController {

    @Autowired
    RestApplicationRepository repository;

    @GetMapping("/")
    public CollectionModel<Tour> getTours() {
        List<Tour> tours = repository.getTours();
        CollectionModel<Tour> response = CollectionModel.of(tours);
        response.add(linkTo(methodOn(RestTourController.class).getTours()).withSelfRel());
        return response;
    }

    @GetMapping("/to-delete")
    public CollectionModel<Tour> getToursToDelete() {
        List<Tour> tours = repository.getToursToDelete();
        CollectionModel<Tour> response = CollectionModel.of(tours);
        response.add(linkTo(methodOn(RestTourController.class).getToursToDelete()).withSelfRel());
        return response;
    }

    @GetMapping("/{id}")
    public HttpEntity<Tour> getTour(@PathVariable String id) {
        Tour tour = repository.getTourById(id);
        if (tour == null) return ResponseEntity.status(HttpStatus.NOT_FOUND).body(null);

        tour.removeLinks();
        tour.add(linkTo(RestTourController.class).slash(id).withSelfRel());
        tour.add(linkTo(methodOn(RestTourController.class).deleteTour(id)).withRel("remove"));
        tour.add(linkTo(methodOn(RestTourController.class).getTours()).withRel("list"));
        return ResponseEntity.status(HttpStatus.OK).body(tour);
    }

    @PostMapping(value = "/")
    public ResponseEntity createTour(@RequestBody Tour tour) {
        repository.addTour(tour);
        return ResponseEntity.status(HttpStatus.CREATED)
                .header("Location", "/tour/" + tour.getId())
                .build();
    }

    @DeleteMapping("/{id}")
    public ResponseEntity deleteTour(@PathVariable String id) {
        repository.deleteTour(id);
        return ResponseEntity.status(HttpStatus.NO_CONTENT)
                .header("Location", "/tour/to-delete")
                .build();
    }

}
