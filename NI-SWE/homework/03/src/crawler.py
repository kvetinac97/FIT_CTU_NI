import scrapy
import json


class SweSpider(scrapy.Spider):
    # initial setup
    name = 'swe-spider'
    allowed_domains = ['dbpedia.org']
    start_urls = ['https://dbpedia.org/sparql']

    # advanced setup
    custom_settings = {
        'USER_AGENT': 'FIT-CVUT-SWE',
        # in robots.txt, the delay is not defined,
        # so I decided to use around 1 second
        'DOWNLOAD_DELAY': 1,
        'RANDOMIZE_DOWNLOAD_DELAY': True,
        # export settings
        'FEED_FORMAT': 'json',
        'FEED_EXPORT_ENCODING': 'utf-8'
    }

    def parse(self, response):
        # Set the initial offset and limit
        offset = 0
        limit = 10
        # Specify the total number of results you want to retrieve
        total_results = 100

        # Iterate until the total number of results is reached
        while offset < total_results:
            # Example SPARQL query with OFFSET and LIMIT
            sparql_query = f"""
                PREFIX dbo: <http://dbpedia.org/ontology/>
                PREFIX dbp: <http://dbpedia.org/property/>
                
                SELECT ?country ?label ?population
                WHERE {{
                    ?country dbp:label ?label ;
                            dbp:populationTotal	?population ;
                            a dbo:Country .
                    FILTER (lang(?label) = 'en') .
                }}
                ORDER BY ?label
                OFFSET {offset}
                LIMIT {limit}
            """

            # Send a POST request to the SPARQL endpoint
            yield scrapy.FormRequest(
                url='https://dbpedia.org/sparql',
                formdata={'query': sparql_query, 'format': 'json'},
                callback=self.parse_results
            )

            # Increment the offset for the next request
            offset += limit

    def parse_results(self, response):
        # Parse the JSON response and process the results
        result = json.loads(response.text)
        bindings = result.get('results', {}).get('bindings', [])

        for binding in bindings:
            country = binding.get('country', {}).get('value', '')
            label = binding.get('label', {}).get('value', '')
            population = binding.get('population', {}).get('value', '')

            yield {
                'country': country,
                'label': label,
                'population': population,
            }
