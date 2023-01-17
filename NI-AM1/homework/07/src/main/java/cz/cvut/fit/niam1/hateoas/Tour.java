package cz.cvut.fit.niam1.hateoas;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.springframework.hateoas.RepresentationModel;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Tour extends RepresentationModel<Tour> {

    String id;
    String name;

    List<Customer> customers;

    // Because of Jackson XML
    public Tour() {
        this(null, null, null);
    }

    public Tour(String id, String name) {
        this(id, name, new ArrayList<>());
    }
    public Tour(String id, String name, List<Customer> customers) {
        this.id = id;
        this.name = name;
        this.customers = customers;
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

    public List<Customer> getCustomers() {
        return customers;
    }

    public void addCustomer(Customer customer) {
        customers.add(customer);
    }

    public void setCustomers(List<Customer> customers) {
        this.customers = customers;
    }

    @JsonIgnore public int getWeakHash() {
        return Objects.hash(id, name);
    }
    // Classic hash is full object fingerprint, customers included

}
