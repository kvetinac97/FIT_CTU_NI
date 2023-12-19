package cz.cvut.fit.niswe

import org.apache.jena.rdf.model.Model
import org.apache.jena.rdf.model.ModelFactory
import org.apache.jena.reasoner.ReasonerRegistry
import org.apache.jena.riot.Lang
import org.apache.jena.riot.RDFWriter
import org.apache.jena.riot.RIOT
import org.apache.jena.vocabulary.RDF
import org.apache.jena.vocabulary.RDFS
import org.apache.jena.vocabulary.XSD
import java.io.FileOutputStream

fun Model.createVocabularyAndData() = apply {
    // Terms prefix
    setNsPrefix("ext", "http://example.com/terms/")
    setNsPrefix("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#")
    setNsPrefix("rdfs", "http://www.w3.org/2000/01/rdf-schema#")

    // Person
    val personClass = createResource("ext:Person")
    // Parent subclass of Person
    val parentClass = createResource("ext:Parent")
    add(parentClass, RDFS.subClassOf, personClass)
    // Child as subclass of Person
    val childClass = createResource("ext:Child")
    add(childClass, RDFS.subClassOf, personClass)
    // Man as subclass of Person
    val manClass = createResource("ext:Man")
    add(manClass, RDFS.subClassOf, personClass)
    // Father as subclass of Parent and Man
    val fatherClass = createResource("ext:Father")
    add(fatherClass, RDFS.subClassOf, parentClass)
    add(fatherClass, RDFS.subClassOf, manClass)
    // Organisation
    val organisationClass = createResource("ext:Organisation")

    // worksFor - relationship between a person and an organisation
    val worksFor = createProperty("ext:worksFor")
    add(worksFor, RDF.type, RDF.Property)
    add(worksFor, RDFS.domain, personClass)
    add(worksFor, RDFS.range, organisationClass)
    // hasFather - relationship between a person and his/her father
    val hasFather = createProperty("ext:hasFather")
    add(hasFather, RDF.type, RDF.Property)
    add(hasFather, RDFS.domain, personClass)
    add(hasFather, RDFS.range, fatherClass)
    // officialName - official name of company
    val officialName = createProperty("ext:officialName")
    add(officialName, RDF.type, RDF.Property)
    add(officialName, RDFS.domain, organisationClass)
    add(officialName, RDFS.range, XSD.normalizedString)
    // age - age of a Person
    val age = createProperty("ext:age")
    add(age, RDF.type, RDF.Property)
    add(age, RDFS.domain, personClass)
    add(age, RDFS.range, XSD.nonNegativeInteger)

    // provide an export (in the Turtle serialisation format)
    // of the RDF graph containing only the vocabulary definitions
    printStatements("hw02-01.ttl")

    // Term prefix
    setNsPrefix("ex", "http://example.com/data/")
    setNsPrefix("xsd", "http://www.w3.org/2001/XMLSchema#")

    // Carl is a Man
    val carl = createResource("ex:Carl")
    add(carl, RDF.type, manClass)

    // Anna hasFather Carl
    val anna = createResource("ex:Anna")
    add(anna, hasFather, carl)

    // Anna worksFor IBM
    val ibm = createResource("ex:IBM")
    add(anna, worksFor, ibm)

    // Anna is 25 years old
    add(anna, age, createTypedLiteral(25))
    // the official name of IBM is International Business Machines Corporation
    add(ibm, officialName, "International Business Machines Corporation")
}

// Create an InfModel with RDFS inferring
fun Model.inferredModel(): Model = run {
    // Create an RDFS reasoner
    ModelFactory.createInfModel(ReasonerRegistry.getRDFSReasoner(), this)
}

fun Model.printStatements(outputFileName: String) = let {
    with(FileOutputStream(outputFileName)) {
        RDFWriter.create()
            .source(it)
            .lang(Lang.TTL)
            .output(this)
    }
}

fun main() {
    // Provide an export (in the Turtle serialisation format)
    // of the initial RDF graph (vocabulary and instance data)
    // (before reasoning)
    val model = ModelFactory.createDefaultModel()
            .createVocabularyAndData()
    model.printStatements("hw02-02.ttl")

    // Provide an export (in the Turtle serialisation format)
    // of the inferred RDF graph (after reasoning)
    val inferredModel = model.inferredModel()
    inferredModel.printStatements("hw02-03.ttl")
}
