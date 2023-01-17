# POST /country
curl -vX POST "http://localhost:8080/country" -H  "Content-Type: application/json" -d "{\"id\":\"PL\",\"name\":\"Polsko\"}"

# GET /country
curl -vX GET "http://localhost:8080/country/PL"

# PUT /country
curl -vX PUT "http://localhost:8080/country/PL" -H "Content-Type: application/json" -d "{\"id\":\"PL\",\"name\":\"Polska\"}"

# DELETE /country
curl -vX DELETE "http://localhost:8080/country/PL"

# POST /tour
curl -vX POST "http://localhost:8080/tour" -H  "Content-Type: application/json" -d "{\"id\":\"3\",\"name\":\"Sand tour\"}"

# GET /tour
curl -vX GET "http://localhost:8080/tour/3"

# PUT /tour
curl -vX PUT "http://localhost:8080/tour/3" -H "Content-Type: application/json" -d "{\"id\":\"3\",\"name\":\"Pisek tour\"}"

# DELETE /tour
curl -vX DELETE "http://localhost:8080/tour/3"
