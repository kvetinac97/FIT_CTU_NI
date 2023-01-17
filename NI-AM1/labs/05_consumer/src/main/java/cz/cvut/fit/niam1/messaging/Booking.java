package cz.cvut.fit.niam1.messaging;

import java.io.Serializable;

public class Booking implements Serializable {

    public String id;
    public String name;

    public Booking(String id, String name) {
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
