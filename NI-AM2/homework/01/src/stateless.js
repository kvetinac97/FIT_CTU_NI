const net = require('net')

// create server
const server = net.createServer((socket) => {
    // socket opened
    console.log('socket opened')
    socket.setEncoding('utf8')

    // socket connection close listener
    socket.on('end', () => {
        console.log('connection/socket closed')
    })

    // keep track of existing orders
    let orders = []

    socket.on('data', (data) => {
        // match command
        const matched = data.trim().match("([a-zA-Z]+) ([a-zA-Z0-9]+) ?([a-zA-Z0-9]+)?")
        switch (matched ? matched[1] : '') {
            case 'open':
                // check opened order
                if (orders[matched[2]] !== undefined) {
                    socket.write(`Order ${matched[2]} is already opened!\n`)
                    return
                }

                // open an empty order
                orders[matched[2]] = []
                socket.write(`Opened order ${matched[2]} \n`)
                break
            case 'add':
                // check third parameter
                if (matched[3] === undefined) {
                    socket.write('Syntax: add <order> <item>\n')
                    return
                }
                // check order opened
                if (orders[matched[2]] === undefined) {
                    socket.write(`Order ${matched[2]} is not opened!\n`)
                    return
                }
                // add item to order
                orders[matched[2]].push(matched[3])
                socket.write(`Added ${matched[3]} to order ${matched[2]}\n`)
                break
            case 'process':
                // check order opened
                if (orders[matched[2]] === undefined) {
                    socket.write(`Order ${matched[2]} is not opened!\n`)
                    return
                }
                // process order and close it
                socket.write(`Processed order ${matched[2]} with items ${orders[matched[2]]}\n`)
                orders[matched[2]] = undefined
                break
            case 'exit':
                // end connection
                socket.end()
                break
            default:
                socket.write('Syntax: <open|process|add> <order> (item)\n')
                break
        }
    })
})

// Start the server (on port 8124)
server.listen(8124, () => {
    console.log('server started')
})
