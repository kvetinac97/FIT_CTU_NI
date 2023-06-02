// http library
const http = require("http");
const redis = require('redis');

// constants
const server_hostname = '0.0.0.0'
const redis_hostname = '10.43.7.29'
const server_port = 8888
const redis_port = 6379

// create client
const client = redis.createClient({
    socket: {
        host: redis_hostname,
        port: redis_port
    }
})

// set up callbacks
client.on('connect', () => {
    console.log('Redis client connected')
})
client.on('error', (err) => {
    console.log(err)
})

// connect to client
client.connect()

// start server
http.createServer((req, res) => {
    // match /person/{name}/address
    const regex = RegExp("^\/person\/([a-zA-Z0-9_]*)\/address$")
    const match = req.url.match(regex)

    // no match = 404
    if (!match) {
        res.statusCode = 404
        res.end()
        return
    }

    // Try to get address from client
    const name = match[1]
    client.get(name)
        .then((result) => {
            // Could not find in DB = error
            if (result === null)
                throw `Error: User ${name} not found.\n`

            // Found successfully = return result
            res.statusCode = 200
            res.end(`Address: ${result}\n`)
        })
        // Error (connection / user not found): print
        .catch((error) => {
            res.statusCode = 200
            res.end(error)
        })
}).listen(server_port, server_hostname, () => {
    console.log(`Server running at http://${server_hostname}:${server_port}/`)
});
