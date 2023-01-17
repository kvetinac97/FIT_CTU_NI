package cz.cvut.fit.wrzecond.model;

import java.math.BigInteger;

public record Booking(
    BigInteger tourId,
    String location,
    Person person
) { }

