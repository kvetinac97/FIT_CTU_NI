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
const fs = require("fs");
const url = require("url");

const hostname = '127.0.0.1';
const port = 8080;

http.createServer((req, res) => {
    let id
    if ((id = req.url.match("^/jsonp")) && req.method === "GET") {
        const queryObj = url.parse(req.url, true).query
        res.writeHead(200, {
            'Content-Type': 'application/javascript'
        })
        res.end(`${queryObj['_callback']}({"a": 1})`)
    }
    else {
        res.writeHead(200, {
            'Content-Type': 'text/html'
        })
        res.end(fs.readFileSync("index.html"))
    }
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});