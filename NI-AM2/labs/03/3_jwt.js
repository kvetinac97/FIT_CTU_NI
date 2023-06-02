const crypto = require('crypto');

let header = {
    typ: 'JWT',
    alg: 'HS256'
};

header = Buffer.from(JSON.stringify(header)).toString('base64');

let payload = {
    iat: Date.now(),
    iss: 'somebody',
    username: 'somebody'
};

payload = Buffer.from(JSON.stringify(payload)).toString('base64');

let key = header + '.' + payload;
let signature = crypto.createHmac('sha256', 'secret');
signature.update(key);
key = signature.digest('base64');

let token = header + '.' +payload + '.' + key
console.log(token)