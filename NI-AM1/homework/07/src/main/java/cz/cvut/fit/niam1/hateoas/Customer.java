package cz.cvut.fit.niam1.hateoas;

import org.springframework.hateoas.RepresentationModel;

public class Customer extends RepresentationModel<Customer> {

    String id;
    String name;

    public Customer(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

}
