const https = require('https')
const fs = require('fs')

const url = require('url')
const axios = require('axios')

const options = {
    key: fs.readFileSync('localhost-privkey.pem'),
    cert: fs.readFileSync('localhost-cert.pem')
}

const CLIENT_ID = 'PUT_CLIENT_ID_HERE'
const CLIENT_SECRET = 'PUT_CLIENT_SECRET_HERE'
const REDIRECT_URI = 'https://localhost:8080/code'

let session = {
    authorization: "https://accounts.google.com/o/oauth2/v2/auth?scope=https%3A//www.googleapis.com/auth/drive.metadata.readonly&include_granted_scopes=true&state=12345&response_type=code&access_type=offline&redirect_uri=https://localhost:8080/code&client_id=FILL_CLIENT_ID_HERE"
}

https.createServer(options, (req, res) => {
    if (req.url === '/favicon.ico')
        return res.end()

    if (req.url === '/') {
        if (session['access_token']) {
            axios.get(`https://www.googleapis.com/drive/v2/files?access_token=${session['access_token']}`)
                .then((response) => {
                    res.end(JSON.stringify(response.data))
                })
                .catch((error) => {
                    res.end(JSON.stringify(error))
                })
        }
        else {
            res.writeHead(200, {'Content-Type': 'application/json'})
            res.end(JSON.stringify(session))
        }
        return
    }

    const queryObject = url.parse(req.url, true).query
    const data = {
        gran_type: 'authorization_code',
        client_id: CLIENT_ID,
        client_secret: CLIENT_SECRET,
        redirect_uri: REDIRECT_URI,
        code: queryObject['code']
    }
    axios.post('https://oauth2.googleapis.com/token', data)
        .then((response) => {
            session['access_token'] = response.data['access_token']
            res.writeHead(301, {'Location': 'https://localhost:8080/'})
            res.end()
        })
        .catch((error) => {
            res.end(JSON.stringify(error))
        })
}).listen(8080);
console.log("Running server")
