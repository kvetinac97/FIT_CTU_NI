const http = require("http");

const hostname = '127.0.0.1';
const port = 8080;

http.createServer((req, res) => {

    // initialize the body to get the data asynchronously
    req.body = "";

    // get the data of the body
    req.on('data', (chunk) => {
        req.body += chunk;
    });

    //all data
    req.on('end', () => {
        // log request object
        console.log(req.body);
        // process information
        // send response to client
        res.end('hello world');
    });
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});