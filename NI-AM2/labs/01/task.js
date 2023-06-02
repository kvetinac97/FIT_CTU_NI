// Includes
const http = require('http');
const url = require("url");
const path = require("path");
const fs = require("fs");
const fsPromise = require("fs/promises")

// Constants
const hostname = '127.0.0.1';
const port = 8080;
const wd = process.cwd();

const ServeStatic = {
    exists: (path, callback) => {
        fs.access(path, fs.constants.F_OK, callback);
    },
    isDir: async (path) => {
        return (await fsPromise.lstat(path)).isDirectory()
    },
    listDir: (path) => {
        return fsPromise.readdir(path)
    },
    readContent: (path, callback) => {
        fs.readFile(path, "binary", callback);
    }
}

http.createServer((req, res) => {
    const pathName = url.parse(req.url).pathname
    const filePath = path.join(wd, pathName)

    ServeStatic.exists(filePath, async (err) => {
        if (err){
            res.statusCode = 404
            res.end()
            return
        }

        if (await ServeStatic.isDir(filePath)) {
            ServeStatic.listDir(filePath)
                .then((result) => {
                    res.statusCode = 200
                    res.setHeader("Content-Type", "application/json")
                    res.write(JSON.stringify(result))
                    res.end()
                })
                .catch(() => {
                    res.statusCode = 400
                    res.end()
                })
            return
        }

        ServeStatic.readContent(filePath, (err, data) => {
            if (err){
                res.statusCode = 400;
                res.write(JSON.stringify(err))
            }
            else {
                res.statusCode = 200;
                res.write(data, "binary");
            }
            res.end();
        });
    });
}).listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});
