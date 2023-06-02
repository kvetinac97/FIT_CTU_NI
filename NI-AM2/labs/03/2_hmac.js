const crypto = require('crypto');
const hash = crypto.createHmac('sha256', 'secret');
hash.update('data to hash');
console.log(hash.digest('base64url')); // safe for HTML, does not use '/'