// libs
const fs = require('fs')
const http = require("http")

// constants
const hostname = '127.0.0.1'
const port = 8080
const seconds = 2

// reading lines TODO: put file.txt with some quote lines here
const lines = fs.readFileSync('file.txt').toString().split('\n').reverse()

// server
http.createServer((req, res) => {
    if (req.url.match("^/messages")) {
        let index = 0
        res.writeHead(200, {
            'Cache-Control': 'no-cache',
            'Content-Type': 'text/event-stream',
            'Connection': 'keep-alive'
        })

        const id = setInterval(() => {
            res.write(`data: ${lines[index++]}\n\n`)

            if (index >= lines.length) {
                clearInterval(id)
                res.end()
            }
        }, seconds * 1000)
        return
    }
    res.writeHead(200, {'Content-Type': 'text/html'})
    res.end(fs.readFileSync('client.html'))
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`)
})
