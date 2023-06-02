// file-system library
const fs = require("fs")
console.log(fs.statSync("1_code.js"))

// http library
const http = require("http");

// constants
const hostname = '127.0.0.1'
const port = 8080

http.createServer((req, res) => {
    // check the value of host header
    // curl -H 'Host: company.cz' -v localhost:8080
    if (req.headers.host === "company.cz") {
        res.writeHead(201, "Content-Type: text/plain");
        res.end();
    }
    else {
        res.statusCode = 200
        res.setHeader("Content-Type", "text/plain")
        res.end("This is the response...\n");
    }
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`)
});