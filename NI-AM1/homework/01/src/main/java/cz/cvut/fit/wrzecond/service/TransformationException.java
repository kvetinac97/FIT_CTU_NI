package cz.cvut.fit.wrzecond.service;

public class TransformationException extends IllegalArgumentException {
    @Override
    public String getMessage() {
        return "Invalid transformation string.";
    }
}
