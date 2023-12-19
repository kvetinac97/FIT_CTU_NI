package com.example

import org.apache.jena.rdf.model.InfModel
import org.apache.jena.rdf.model.ModelFactory
import org.apache.jena.rdf.model.RDFNode
import org.apache.jena.reasoner.Reasoner
import org.apache.jena.reasoner.ReasonerRegistry
import org.apache.jena.vocabulary.RDF
import org.apache.jena.vocabulary.RDFS
import org.apache.jena.vocabulary.XSD

fun main() {
    // Create an empty RDF model

    // Create an empty RDF model
    val model = ModelFactory.createDefaultModel()

    // Define namespaces

    // Define namespaces
    val exNS = "http://example.org"
    val extNS = "http://example.org/type"

    val personClass = model.createResource("$extNS/Person")
    val programmer = model.createResource("$extNS/Programmer")
    model.add(programmer, RDFS.subClassOf, personClass)

    val nameProperty = model.createProperty("$extNS/name")
    model.add(nameProperty, RDF.type, RDF.Property)
    model.add(nameProperty, RDFS.domain, personClass)
    model.add(nameProperty, RDFS.range, XSD.normalizedString)

    val ageProperty = model.createProperty("$extNS/age")
    model.add(ageProperty, RDF.type, RDF.Property)
    model.add(ageProperty, RDFS.domain, personClass)
    model.add(ageProperty, RDFS.range, XSD.nonNegativeInteger)

    val emailProperty = model.createProperty("$extNS/mail")
    model.add(emailProperty, RDF.type, RDF.Property)
    model.add(emailProperty, RDFS.domain, personClass)
    model.add(emailProperty, RDFS.range, XSD.normalizedString)

    val john = model.createResource("$exNS/John")
    model.add(john, RDF.type, programmer)
    model.add(john, nameProperty, "John")
    model.add(john, ageProperty, model.createTypedLiteral(14))
    model.add(john, emailProperty, "john@example.org")

    val jane = model.createResource("$exNS/Jane")
    model.add(jane, RDF.type, personClass)
    model.add(jane, nameProperty, "Jane")
    model.add(jane, ageProperty, model.createTypedLiteral(27))
    model.add(jane, emailProperty, "jane@example.org")


    // Create an RDFS reasoner
    val reasoner = ReasonerRegistry.getRDFSReasoner()

    // Create an InfModel with RDFS inferencing
    val infModel = ModelFactory.createInfModel(reasoner, model)

    val iter = infModel.listStatements(null, null, null as RDFNode?)

    while (iter.hasNext()) {
        val stmt = iter.next()
        println(stmt)
    }
}