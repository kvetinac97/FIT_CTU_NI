package cz.cvut.fit.niam1.hateoas;


import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;

@Component
public class RestApplicationRepository {

    private static final List<Tour> tours = new ArrayList<>();
    private static Long lastModified = 0L;

    @PostConstruct
    public void initRepo() {
        tours.add(new Tour("1", "Summer trip"));

        List<Customer> customers = new ArrayList<>();
        customers.add(new Customer( "1", "Josef"));
        customers.add(new Customer("2", "Tomáš"));
        tours.add(new Tour("2", "Eiffel tower visit", customers));
        tours.add(new Tour("3", "Prague visit"));
        updateLastModified();
    }

    public void addTour(Tour tour) {
        int max = tours.stream().map(t-> Integer.valueOf(t.getId())).max(Integer::compare).get();
        tour.setId(String.valueOf(max+1));
        tours.add(tour);
        updateLastModified();
    }

    public void addTourCustomer(String id, Customer customer) {
        Tour t = getTourById(id);
        if (t == null) return;
        t.addCustomer(customer);
        updateLastModified();
    }

    public List<Tour> getTours() {
        return tours;
    }

    public Tour getTourById(String id) {
        return tours.stream().filter(t -> t.getId().equals(id)).findAny().orElse(null);
    }

    public void deleteTour(String id) {
        tours.removeIf(t -> t.getId().equals(id));
        updateLastModified();
    }

    public void updateLastModified() {
        lastModified = System.currentTimeMillis();
    }

    public long getLastModified() {
        return lastModified;
    }

}
