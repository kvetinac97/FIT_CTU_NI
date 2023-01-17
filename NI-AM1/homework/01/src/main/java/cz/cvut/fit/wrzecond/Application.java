package cz.cvut.fit.wrzecond;

import cz.cvut.fit.wrzecond.service.TransformationService;

/**
 * Example application with example input and output
 */
public class Application {
    public static void main(String[] args) {
        String exampleInput = """
                Dear Sir or Madam,

                please find the details about my booking below:

                ===
                Tour id: "1"
                Location: "Bohemian Switzerland"
                Person: "Jan Karel Novak"
                ===

                Regards,
                Jan Novak""";

        String exampleOutput = TransformationService.textToJson(exampleInput);
        System.out.println(exampleOutput);
    }
}
