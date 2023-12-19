import requests
from rdflib import Graph, URIRef, OWL


def call_sparql(endpoint, query):
    params = {'query': query, 'format': 'text/turtle'}
    response = requests.get(endpoint, params=params, headers={'Accept': 'text/turtle'})
    return response.text


def load_dbpedia_graph():
    dbpedia_url = "https://dbpedia.org/sparql"
    dbpedia_sparql_query = """
    PREFIX ex: <http://example.org/>
    CONSTRUCT {
        ?player ex:name ?name ;
            ex:birthPlaceLabel ?birthPlace ;
            ex:birthPlace ?birthPlaceResource
    }
    WHERE {
      ?player rdf:type dbo:SoccerPlayer;
              foaf:name ?name;
              dbo:birthPlace ?birthPlaceResource.
      ?birthPlaceResource rdfs:label ?birthPlace ;
              rdf:type dbo:City .
      FILTER(LANG(?birthPlace) = 'en' AND LANG(?name) = 'en')
    }
    LIMIT 20000
    """
    dbpedia_data = call_sparql(dbpedia_url, dbpedia_sparql_query)
    dbpedia_graph = Graph().parse(data=dbpedia_data, format="turtle")
    print("Parsed DBpedia graph")
    return dbpedia_graph


def load_wikidata_graph():
    wikidata_url = "https://query.wikidata.org/sparql"
    wikidata_sparql_query = """
    PREFIX ex: <http://example.org/>
    CONSTRUCT {
        ?player ex:name ?name ;
            ex:birthPlaceLabel ?birthPlace ;
            ex:birthPlace ?birthPlaceResource
    }
    WHERE {
      ?player wdt:P106 wd:Q937857.
      ?player rdfs:label ?name.
      ?player wdt:P19 ?birthPlaceResource.
      ?birthPlaceResource rdfs:label ?birthPlace.
      FILTER(LANG(?name) = 'en' && LANG(?birthPlace) = 'en')
    }
    LIMIT 1000
    """
    wikidata_data = call_sparql(wikidata_url, wikidata_sparql_query)
    wikidata_graph = Graph().parse(data=wikidata_data, format="xml")
    print("Parsed Wikidata")
    return wikidata_graph


# Function to generate links between entities from DBPedia and Wikidata
def generate_links():
    entity_map = {}
    graph1 = load_dbpedia_graph()
    graph2 = load_wikidata_graph()
    for entity1 in graph1.subjects():
        name1 = graph1.value(entity1, URIRef("http://example.org/name"))
        birth1 = graph1.value(entity1, URIRef("http://example.org/birthPlaceLabel"))
        entity_map[(str(name1), str(birth1))] = entity1
    for entity2 in graph2.subjects():
        name2 = graph2.value(entity2, URIRef("http://example.org/name"))
        birth2 = graph2.value(entity2, URIRef("http://example.org/birthPlaceLabel"))
        birth2place = graph2.value(entity2, URIRef("http://example.org/birthPlace"))
        if (str(name2), str(birth2)) in entity_map:
            ent1 = entity_map[(str(name2), str(birth2))]
            print((ent1, OWL.sameAs, entity2))
            print((ent1, URIRef("http://dbpedia.org/ontology/birthPlace"), birth2place))


generate_links()
