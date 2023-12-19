curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=json' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX restaurant: <http://localhost:8888/restaurant/>
                           PREFIX table: <http://localhost:8888/table/>
                           PREFIX table_type: <http://localhost:8888/table_type/>
                           PREFIX order: <http://localhost:8888/order/>
                           PREFIX food: <http://localhost:8888/food/>

                           DESCRIBE ?table
                           WHERE {
                               {
                                   SELECT ?table ?table_type (COUNT(?food) AS ?food_count) WHERE {
                                       ?restaurant restaurant:name ?restaurant_name .
                                       ?table table:in ?restaurant ;
                                              table:type ?type .
                                       ?type table_type:name ?table_type .
                                       ?order order:at ?table ;
                                              order:contains ?food
                                       FILTER (?restaurant_name = "Síla chuti")
                                       {
                                           SELECT (COUNT(?food) AS ?food_total) WHERE {
                                               ?food food:id ?food_id
                                           }
                                       }
                                   }
                                   GROUP BY ?table ?table_type ?food_total HAVING (?food_count = ?food_total)
                                   ORDER BY ?table
                                   LIMIT 10
                               }
                           }'