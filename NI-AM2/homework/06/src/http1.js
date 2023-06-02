// Include packages
const http = require('http');
const fs = require('fs');

// Server constants
const hostname = "localhost"
const port = 8080

// Simple server, which returns files from the filesystem
http.createServer((req, res) => {
    if (req.url === '/')
        return res.end(fs.readFileSync('index.html'))

    if (fs.existsSync(__dirname + req.url))
        res.end(fs.readFileSync(__dirname + req.url))
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`)
});
