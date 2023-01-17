package cz.cvut.fit.niam1.wsserver;

import https.courses_fit_cvut_cz.ni_am1.tutorials.web_services.*;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;

@Component
public class WebServiceRepository {

    private static final List<Tour> tours = new ArrayList<>();

    @PostConstruct
    public void initRepo(){
        Tour t1 = new Tour();
        t1.setName("Beach Tour");
        t1.setType(TourType.BEACH);
        tours.add(t1);
        Tour t2 = new Tour();
        t2.setName("Ski Tour");
        t2.setType(TourType.SKI);
        tours.add(t2);
    }

    public void addTour(Tour t){
        tours.add(t);
    }

    public List<Tour> getTours(){
        return tours;
    }

    public void deleteTour(String name){
        tours.removeIf(t -> t.getName().equals(name));
    }


}
