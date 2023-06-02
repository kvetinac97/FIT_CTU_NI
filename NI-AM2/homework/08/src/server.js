const http2 = require('http2')
const fs = require('fs')

const server = http2.createSecureServer({
    key: fs.readFileSync('localhost-privkey.pem'),
    cert: fs.readFileSync('localhost-cert.pem')
})
server.on('error', (err) => console.error(err))

// Simple HTTPS server returning client.js and index.html
server.on('stream', (stream, headers) => {
    if (headers[":path"] === '/client.js') {
        stream.respond({
            'content-type': 'script/javascript',
            ':status': 200
        })
        return stream.end(fs.readFileSync('client.js'))
    }

    stream.respond({
        'content-type': 'text/html',
        ':status': 200
    })
    return stream.end(fs.readFileSync('index.html'))
})

server.listen(8080)
