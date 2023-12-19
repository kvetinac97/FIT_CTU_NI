curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=text/turtle' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX customer: <http://localhost:8888/customer/>
                           PREFIX table: <http://localhost:8888/table/>
                           PREFIX order: <http://localhost:8888/order/>
                           PREFIX restaurant: <http://localhost:8888/restaurant/>

                           CONSTRUCT {
                               ?customer customer:name ?name ;
                                         customer:sits_at_id ?table_id ;
                                         customer:sits_in_name ?restaurant_name ;
                                         customer:unique_food_count ?food_count
                           }
                           WHERE {
                               SELECT ?customer ?name ?table_id ?restaurant_name (COUNT(?order) AS ?food_count) WHERE {
                                   ?customer customer:first_name ?first_name ;
                                             customer:last_name ?last_name ;
                                             customer:sits_at ?table ;
                                             customer:sits_in/restaurant:name ?restaurant_name .
                                   ?table table:id ?table_id .
                                   OPTIONAL { ?order order:at ?table }
                                   BIND (CONCAT(?first_name, " ", ?last_name) AS ?name)
                               }
                               GROUP BY ?customer ?name ?table_id ?restaurant_name
                           }'