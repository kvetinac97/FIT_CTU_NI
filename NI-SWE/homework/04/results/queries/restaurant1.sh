curl -H 'Accept: application/rdf+xml' -v http://localhost:8888/restaurant/1
# *   Trying 127.0.0.1:8888...
# * Connected to localhost (127.0.0.1) port 8888 (#0)
# > GET /ingredient/31 HTTP/1.1
# > Host: localhost:8888
# > User-Agent: curl/8.1.2
# > Accept: application/rdf+xml
# >
# < HTTP/1.1 303 See Other
# < Location: http://127.0.0.1:8888/ingredient/31.xml
# < Date: Tue, 14 Nov 2023 15:57:01 GMT
# < Connection: keep-alive
# < Keep-Alive: timeout=5
# < Content-Length: 0
# <
# * Connection #0 to host localhost left intact
curl -H 'Accept: application/rdf+xml' -v http://localhost:8888/restaurant/1.xml > restaurant1.xml