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

    // log request object
    console.log(req);
    // write head and status code
    res.writeHead(200, {'Content-Type': 'text/plain'});
    // send response to client
    res.end('hello world');

}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});