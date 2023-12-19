curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=json' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX restaurant: <http://localhost:8888/restaurant/>
                           PREFIX manager: <http://localhost:8888/manager/>

                           SELECT ?restaurant ?restaurant_id ?restaurant_name (CONCAT(?first_name, " ", ?last_name) AS ?manager_name)
                           WHERE {
                               ?restaurant restaurant:id ?restaurant_id ; restaurant:name ?restaurant_name ;
                               restaurant:managed_by ?manager . ?manager manager:first_name ?first_name ;
                               manager:last_name ?last_name . FILTER (?first_name != "Vendelín")
                           }
                           ORDER BY ?restaurant_id LIMIT 10'