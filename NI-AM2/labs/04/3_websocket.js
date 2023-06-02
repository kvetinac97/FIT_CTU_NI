const port = 8080

let WebSocketServer = require('ws').Server, wss = new WebSocketServer({ port: port });

wss.on('connection', function connection(ws) {
    ws.on('message', function incoming(message) {
        wss.clients.forEach(function each(client) {
            client.send(JSON.parse(message));
        });
    });

    ws.send('Connected');
});
