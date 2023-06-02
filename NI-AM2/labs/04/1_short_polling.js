const http = require('http')
const fs = require('fs')

const hostname = '127.0.0.1'
const port = 8080

let messages = []

http.createServer((req, res) => {
    req.body = ""
    req.on('data', (chunk) => {
        req.body += chunk
    })
    req.on('end', () => {
        if ((id = req.url.match("^/messages"))) {
            if (req.method === "GET") {
                res.writeHead(200, { 'Content-Type': 'application/json' })
                res.end(JSON.stringify(messages))
                return
            }
            if (req.method === "POST")
                messages.unshift(JSON.parse(req.body))
            res.writeHead(200, { 'Content-Type': 'text/plain' })
            res.end('Default response\n')
            return
        }
        res.writeHead(200, { 'Content-Type': 'text/html'})
        res.end(fs.readFileSync('client.html'))
    })
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`)
})