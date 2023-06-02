const http = require("http");

const hostname = '127.0.0.1';
const port = 8888;

http.createServer((req, res) => {
    switch (req.method + req.url) {
        case 'GET/get/':
            res.writeHead(200, {
                'Content-Type': 'application/text',
                'Access-Control-Allow-Origin': '*'
            })
            res.end("GET")
            break
        case 'POST/post/':
        case 'PUT/put/':
        case 'DELETE/delete/':
        case 'OPTIONS/post/':
        case 'OPTIONS/put/':
        case 'OPTIONS/delete/':
            res.writeHead(200, {
                'Content-Type': 'application/text',
                'Access-Control-Allow-Origin': 'https://users.fit.cvut.cz',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET, POST, PUT, DELETE, OPTIONS'
            })
            res.end(req.method + " " + new Date())
            break
        default:
            res.status = 500
            res.end(`Only GET, POST, PUT, DELETE && OPTIONS are allowed! <br> Got ${req.method + req.url}`)
            break
    }
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});