const http2 = require('http2')
const fs = require('fs')

// Create HTTP/2 server
const server = http2.createSecureServer({
    key: fs.readFileSync('localhost-privkey.pem'),
    cert: fs.readFileSync('localhost-cert.pem')
})

// Send file function
const sendFile = (stream, path) => {
    const fd = fs.openSync(path, "r")
    const stat = fs.fstatSync(fd)
    stream.respondWithFD(fd, {"content-length": stat.size})
    stream.on("close", () => fs.closeSync(fd))
}

// HTTP/2 push
const pushImage = (stream, id) => {
    const path = `/img/cat${id}.jpeg` // TODO: put lot of cat images in /img
    console.log(`Pushing ${path}`)
    stream.pushStream({":path": path}, (err, ps) => {
        if (err) return console.log(err)
        sendFile(ps, __dirname + path)
        ps.end()
    });
}

// Stream listener
server.on('stream', (stream, headers) => {
    // if not on index, send the file
    const path = headers[":path"]
    if (path !== '/' && path !== '/index.html')
        if (path.substring(-1) !== '/' && fs.existsSync(__dirname + path)) { // is not directory
            console.log(`requested ${path}`)
            return sendFile(stream, __dirname + path)
        }
        else
            return

    // try to push all images when on '/' path
    if (stream.pushAllowed)
        for (let id = 200; id > 0; --id)
            pushImage(stream, id)

    // send index.html file
    sendFile(stream, 'index.html')
});

// start server
server.listen(8443);
