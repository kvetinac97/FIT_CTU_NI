const http = require('http')
const fs = require('fs')

const hostname = '127.0.0.1'
const port = 8080

let messages = []
let clients = []

http.createServer((req, res) => {
    req.body = ""
    req.on('data', (chunk) => {
        req.body += chunk
    })
    req.on('end', () => {
        if ((id = req.url.match("^/messages"))) {
            if (req.method === "GET") {
                clients.push(res)
                return
            }
            if (req.method === "POST") {
                messages.unshift(JSON.parse(req.body))
                for (let i = clients.length - 1; i >= 0; i--) {
                    let client = clients[i]
                    client.writeHead(200, { 'Content-Type': 'application/json' })
                    client.end(JSON.stringify(messages))
                    clients.pop()
                }
            }
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
