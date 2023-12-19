curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=json' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX restaurant: <http://localhost:8888/restaurant/>

                           ASK WHERE { FILTER NOT EXISTS { SELECT * WHERE { # Reverse yes/no
                               ?restaurant restaurant:name ?name .
                               OPTIONAL { ?restaurant restaurant:managed_by ?manager }
                           }
                           GROUP BY ?restaurant ?name HAVING (COUNT(?manager) != 1) } }'