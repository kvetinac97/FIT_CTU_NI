const http = require("http");

const hostname = '127.0.0.1';
const port = 8080;

const crypto = require('crypto');
const secret = "jwt_secret"
const secret2 = "hmac_secret"

let jwt = {
    sign: (username, secret) => {
        let header = {
            typ: 'JWT',
            alg: 'HS256'
        };

        header = Buffer.from(JSON.stringify(header)).toString('base64');

        let payload = {
            iat: Date.now(),
            username: username,
            expiration: new Date((new Date()).getTime() + 5 * 60 * 1000) // 5 minutes
        };

        payload = Buffer.from(JSON.stringify(payload)).toString('base64');

        let key = header + '.' + payload
        let signature = crypto.createHmac('sha256', secret)
        signature.update(key)
        key = signature.digest('base64')

        return header + '.' +payload + '.' + key
    },
    verify: (token, secret) => {
        let parts = token.split('.')
        let header = Buffer.from(parts[0], 'base64').toString('ascii')
        let payload = Buffer.from(parts[1], 'base64').toString('ascii')
        console.log(header, payload)
        let payloadData = JSON.parse(payload)

        console.log(new Date(payloadData.expiration), new Date())
        if (new Date(payloadData.expiration) < new Date())
            return [false, null]

        let key = parts[0] + '.' + parts[1]
        let signature = crypto.createHmac('sha256', secret)
        signature.update(key)
        key = signature.digest('base64')
        return [parts[2] === key, payloadData.username]
    }
};

let hmac = {
    compute: (request, secret) => {
        let signature = crypto.createHmac('sha256', secret)
        let data = `${request.method}-${request.url}-${request.headers["host"]}-${request.headers["user-agent"]}`
        signature.update(data)
        return signature.digest('base64')
    }
}

http.createServer((req, res) => {
    req.body = "";
    req.on('data', (chunk) => {
        req.body += chunk;
    });
    req.on('end', () => {
        if (req.url === '/login' && req.method === 'POST') {
            let username = JSON.parse(req.body)["username"]
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify({"token": jwt.sign(username, secret)}));
        }

        if (req.url === '/refresh' && req.method === 'POST') {
            if (!jwt.verify(req.headers.authorization, secret)) {
                res.writeHead(401)
                res.end()
                return
            }
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify({"token": jwt.sign(secret)}));
        }

        if (req.url === '/hmac' && req.method === 'GET') {
            res.writeHead(200, {'Content-Type': 'application/json'})
            res.end(JSON.stringify({"signature": hmac.compute(req, secret2)}))
        }

        if (req.url === '/data' && req.method === 'GET') {
            console.log(req.headers);
            let [success, username] = jwt.verify(req.headers.authorization, secret)
            if (!success) {
                res.writeHead(401)
                res.end()
                return
            }
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify({"username": username}));
        }
        res.writeHead(400);
        res.end();
    });
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});
