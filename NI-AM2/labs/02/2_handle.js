var storage = {
    countries : [{id:"cz", name: "Czech Republic"}],

    getCountry : function(id) {
        for (let i = 0; i < this.countries.length; i++)
            if (this.countries[i].id == id)
                return this.countries[i];
        return null;
    }

};

const http = require("http");

const hostname = '127.0.0.1';
const port = 8080;

http.createServer((req, res) => {
    let id
    if ((id = req.url.match("^/hello/([a-z]+)$"))) {
        if (req.method == "GET") {
            res.writeHead(200, {'Content-Type': 'text/plain'});
            res.end('Hello '+id[1]);
        }
    } else if ((id = req.url.match("^/country/([a-z]+)$"))) {
        if (req.method == "GET") {
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify(storage.getCountry(id[1])));
        }
    } else {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end('Root');
    }
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});
