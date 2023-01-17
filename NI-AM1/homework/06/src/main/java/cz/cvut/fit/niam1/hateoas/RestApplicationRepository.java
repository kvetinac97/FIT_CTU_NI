package cz.cvut.fit.niam1.hateoas;


import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

@Component
public class RestApplicationRepository {

    private static final List<Tour> tours = new ArrayList<>();
    private static final List<Tour> toursToDelete = new ArrayList<>();

    @PostConstruct
    public void initRepo() {
        tours.add(new Tour("1", "Summer trip"));
        tours.add(new Tour("2", "Eiffel tower visit"));
    }

    public void addTour(Tour tour) {
        int max = tours.stream().map(t-> Integer.valueOf(t.getId())).max(Integer::compare).get();
        tour.setId(String.valueOf(max+1));
        tours.add(tour);
    }

    public List<Tour> getTours() {
        return tours;
    }

    public List<Tour> getToursToDelete() { return toursToDelete; }

    public Tour getTourById(String id) {
        return tours.stream().filter(t -> t.getId().equals(id)).findAny().orElse(null);
    }

    public void deleteTour(String id) {
        Tour tour = getTourById(id);
        if (tour == null) return;

        toursToDelete.add(tour);
        tours.removeIf(t -> t.getId().equals(id));

        new Timer().schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        toursToDelete.removeIf(t -> t.getId().equals(id));
                    }
                }, 10000
        );
    }

}
