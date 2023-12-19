<http://example.org/CzechRepublic> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Country> .
<http://example.org/CzechRepublic> <http://example.org/countryProp> <http://example.org/Landlocked> .
<http://example.org/CzechRepublic> <http://example.org/in> <http://example.org/CentralEurope> .
<http://example.org/CzechRepublic> <http://example.org/historicalName> "Bohemia" .
<http://example.org/CzechRepublic> <http://example.org/southBorder> <http://example.org/Austria> .
<http://example.org/CzechRepublic> <http://example.org/westBorder> <http://example.org/Germany> .
<http://example.org/CzechRepublic> <http://example.org/northBorder> <http://example.org/Poland> .
<http://example.org/CzechRepublic> <http://example.org/eastBorder> <http://example.org/Slovakia> .
<http://example.org/CzechRepublic> <http://example.org/landscape> _:landscape1 .
<http://example.org/CzechRepublic> <http://example.org/climate> <http://example.org/ContinentalOceanicClimate> .
<http://example.org/CzechRepublic> <http://example.org/largestCity> <http://example.org/Prague> .
<http://example.org/CzechRepublic> <http://example.org/capital> <http://example.org/Prague> .
<http://example.org/CzechRepublic> <http://example.org/majorCity> <http://example.org/Brno> .
<http://example.org/CzechRepublic> <http://example.org/majorCity> <http://example.org/Ostrava> .
<http://example.org/CzechRepublic> <http://example.org/majorCity> <http://example.org/Plzen> .
<http://example.org/CzechRepublic> <http://example.org/majorCity> <http://example.org/Liberec> .

<http://example.org/Landlocked> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/CountryProperty> .

<http://example.org/CentralEurope> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Region> .

<http://example.org/Austria> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Country> .
<http://example.org/Germany> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Country> .
<http://example.org/Poland> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Country> .
<http://example.org/Slovakia> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Country> .

<http://example.org/ContinentalOceanicClimate> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Climate> .
<http://example.org/ContinentalOceanicClimate> <http://example.org/type> <http://example.org/Continental> .
<http://example.org/ContinentalOceanicClimate> <http://example.org/type> <http://example.org/Oceanic> .

<http://example.org/Continental> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/ClimateType> .
<http://example.org/Oceanic> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/ClimateType> .

_:landscape1 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/Landscape> .
_:landscape1 <http://example.org/type> <http://example.org/Hilly> .
_:landscape1 <http://example.org/covers> _:area78sqkm .
_:landscape1 <http://example.org/covers> _:area30sqmi .

<http://example.org/Hilly> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.org/LandscapeType> .

_:area78sqkm <http://www.w3.org/1999/02/22-rdf-syntax-ns#value> "78871"^^<http://www.w3.org/2001/XMLSchema#integer> .
_:area78sqkm <http://example.org/unit> <http://example.org/squareKilometers> .

_:area30sqmi <http://www.w3.org/1999/02/22-rdf-syntax-ns#value> "30452"^^<http://www.w3.org/2001/XMLSchema#integer> .
_:area30sqmi <http://example.org/unit> <http://example.org/squareMiles> .
