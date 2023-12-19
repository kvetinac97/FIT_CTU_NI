curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=text/turtle' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX table: <http://localhost:8888/table/>
                           PREFIX order: <http://localhost:8888/order/>

                           CONSTRUCT { ?table table:has_food_count ?sum_food } WHERE {
                               {
                                   SELECT ?table (SUM(?food_count) AS ?sum_food) WHERE {
                                       ?order order:at ?table ;
                                              order:count ?food_count
                                   } GROUP BY ?table
                               	LIMIT 10 OFFSET 10
                               }
                           }'