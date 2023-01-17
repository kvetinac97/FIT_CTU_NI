package cz.cvut.fit.niam1;

import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XMLParser {

    public static void main(String[] args) throws Exception {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        Document document = builder.parse(new File("records.xml"));

        NodeList nodeList = document.getElementsByTagName("record");
        for (int i = 0; i < nodeList.getLength(); i++) {
            Node node = nodeList.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE) {
                Element element = (Element) node;
                System.out.println(element.getElementsByTagName("id").item(0).getTextContent());
                if (element.getAttribute("type").equals("trip")) {
                    System.out.println(element.getElementsByTagName("location").item(0).getTextContent());
                    System.out.println(element.getElementsByTagName("capacity").item(0).getTextContent());
                    System.out.println(element.getElementsByTagName("occupied").item(0).getTextContent());
                }
            }
        }
    }

}