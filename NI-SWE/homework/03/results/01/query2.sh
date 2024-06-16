curl -v -X POST "http://localhost:3030/hw03/sparql" \
   -H "Content-Type: application/x-www-form-urlencoded" \
   --data-urlencode 'format=json' \
   --data-urlencode 'default-graph-uri=' \
   --data-urlencode 'query=PREFIX food: <http://localhost:8888/food/>
                           PREFIX food_ingredient: <http://localhost:8888/food_ingredient/>
                           PREFIX ingredient: <http://localhost:8888/ingredient/>

                           SELECT DISTINCT ?ingredient ?ingredient_name
                           WHERE {
                               ?food food:name ?food_name .
                               ?food_ingredient food_ingredient:makes ?food ;
                                                food_ingredient:unit ?unit ;
                                                food_ingredient:count ?ingredient_count ;
                                                food_ingredient:with ?ingredient .
                               ?ingredient ingredient:name ?ingredient_name
                               FILTER (?food_name = "Hovězí vývar s celestýnskými nudlemi" || ?food_name = "Hráškový krém s opečeným chlebíčkem")
                           }
                           LIMIT 10'