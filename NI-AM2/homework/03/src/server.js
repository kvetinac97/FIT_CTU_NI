// http library
const http = require("http");

// constants
const hostname = '0.0.0.0'
const port = 8888

http.createServer((req, res) => {
    const regex = RegExp("^\/([a-zA-Z0-9_]*)$")
    const match = req.url.match(regex)
    if (!match) {
        res.statusCode = 404
        res.end()
        return
    }

    res.statusCode = 200
    const name = match[1]
    console.log(`Response to: ${name}`)
    res.end(`Hello ${name}\n`)
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`)
});
