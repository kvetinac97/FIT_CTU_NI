// server.js - a simple http server implementation
const http = require('http')

const hostname = '127.0.0.1'
const port = 8888
const sparqlEndpoint = 'http://localhost:3030/hw03/sparql'

const contentTypes = {
    ttl: 'text/turtle',
    xml: 'application/rdf+xml',
    json: 'application/rdf+json',
    csv: 'text/csv',
    tsv: 'text/tsv',
    txt: 'text/plain'
}
const suffixes = {
    'text/turtle': 'ttl',
    'application/rdf+xml': 'xml',
    'application/xml': 'xml',
    'application/rdf+json': 'json',
    'application/json': 'json',
    'text/csv': 'csv',
    'text/tsv': 'tsv',
    'text/plain': 'txt'
}

const matchPattern = async (entityType, req, res) => {
    const pattern = new RegExp(`^\/${entityType}\/(.*)$`)
    if (!pattern.test(req.url))
        return false

    const accept = (req.headers['accept'] ?? "").split(",")
    console.log(accept)

    const stringId = req.url.match(pattern)[1]
    console.log(`matched ${stringId}`)
    if (stringId.includes(".")) {
        const realId = stringId.split(".")[0]
        const suffix = stringId.split(".")[1]

        let contentType = contentTypes[suffix]
        if (!contentType) {
            res.statusCode = 406 // unacceptable
            res.end()
            return true
        }
        let headerSuffix = null
        for (let f of accept) {
            let format = f.split(";")[0]
            headerSuffix = suffixes[format]
            if (headerSuffix)
                break
        }

        if (headerSuffix !== suffix) {
            res.statusCode = 406
            res.end()
            return true
        }

        const headers = new Headers()
        headers.append('Accept', contentType)
        headers.append('Content-Type', 'application/x-www-form-urlencoded')

        const body = new URLSearchParams()
        body.append('default-graph-uri', '')
        body.append('query', `PREFIX ${entityType}: <http://localhost:8888/${entityType}/>\n` +
            "\n" +
            `                           DESCRIBE ?${entityType}\n` +
            "                           WHERE {\n" +
            `                               ?${entityType} ${entityType}:id ${realId} ;\n` +
            "                           }")

        let response = await fetch(sparqlEndpoint, {
            method: "POST",
            headers: headers,
            body: body
        })

        res.setHeader('Content-Type', contentType)
        res.end(await response.text())
        return true
    }

    let suffix = null
    for (let f of accept) {
        let format = f.split(";")[0]
        suffix = suffixes[format]
        if (suffix)
            break
    }

    res.statusCode = suffix ? 303 : 406 // See Other || Unacceptable
    if (suffix)
        res.setHeader('Location', `http://${hostname}:${port}/${entityType}/${stringId}.${suffix}`)
    res.end()
    return true
}


const server = http.createServer(async (req, res) => {
    for (let entityType of ['restaurant', 'food', 'manager', 'ingredient', 'employee', 'table',
        'customer', 'order', 'storage_item', 'table_type']) {
        if (await matchPattern(entityType, req, res))
            return
    }

    // Set response headers
    res.statusCode = 404;
    res.end();
});

server.listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});
