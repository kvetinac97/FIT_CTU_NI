// http library
const http = require("http")

// constants
const server_hostname = '0.0.0.0'
const server_port = 8081

// start server
http.createServer((req, res) => {
    res.statusCode = 200
    res.end("Hello!\n")
}).listen(server_port, server_hostname, () => {
    console.log(`Server running at http://${server_hostname}:${server_port}/`)
});
