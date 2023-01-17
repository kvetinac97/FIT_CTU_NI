package cz.cvut.fit.niam1.hateoas;


import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;

@Component
public class RestApplicationRepository {

    private static final List<Country> countries = new ArrayList<>();

    @PostConstruct
    public void initRepo() {
        countries.add(new Country("cz", "Czech Republic"));
        countries.add(new Country("d", "Germany"));
    }

    public void addCountry(Country c) {
        int max = countries.stream().map(t-> Integer.valueOf(t.getId())).max(Integer::compare).get();
        c.setId(String.valueOf(max+1));
        countries.add(c);
    }

    public List<Country> getCountries() {
        return countries;
    }

    public Country getCountryById(String id) {
        return countries.stream().filter(c -> c.getId().equals(id)).findAny().orElse(null);
    }

    public void deleteCountry(String id) {
        countries.removeIf(c -> c.getId().equals(id));
    }

}
