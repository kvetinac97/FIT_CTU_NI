package cz.cvut.fit.wrzecond.service;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import cz.cvut.fit.wrzecond.model.Booking;
import cz.cvut.fit.wrzecond.model.Person;

import java.math.BigInteger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TransformationService {

    // === PUBLIC INTERFACE

    public static String textToJson(String text) throws TransformationException {
        Matcher m = JSON_PATTERN.matcher(text);
        if (!m.find()) throw new TransformationException();

        BigInteger id = new BigInteger((m.group(1)));
        String location = m.group(2), name = m.group(3), surname = m.group(4);

        Booking booking = new Booking(id, location, new Person(name, surname));
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        return gson.toJson(booking);
    }

    // === CONSTANTS ===

    private static final Pattern JSON_PATTERN = Pattern.compile("""
        ===
        Tour id: "(\\d*)"
        Location: "(.*)"
        Person: "(.*) (.*)"
        ==="""
    );

}
