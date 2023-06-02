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

    // keep track of current order
    let order = undefined

    socket.on('data', (data) => {
        // match command
        const matched = data.trim().match("([a-zA-Z]+) ?([a-zA-Z0-9]+)?")
        switch (matched ? matched[1] : '') {
            case 'open':
                // check opened order
                if (order !== undefined) {
                    socket.write(`Order is already opened!\n`)
                    return
                }

                // open an empty order
                order = []
                socket.write(`Opened new order \n`)
                break
            case 'add':
                // check second parameter
                if (matched[2] === undefined) {
                    socket.write('Syntax: add <item>\n')
                    return
                }
                // check order opened
                if (order  === undefined) {
                    socket.write(`Order is not opened!\n`)
                    return
                }
                // add item to order
                order.push(matched[2])
                socket.write(`Added ${matched[2]} to order\n`)
                break
            case 'process':
                // check order opened
                if (order === undefined) {
                    socket.write(`Order is not opened!\n`)
                    return
                }
                // process order and close it
                socket.write(`Processed order with items ${order}\n`)
                order = undefined
                break
            case 'exit':
                // end connection
                socket.end()
                break
            default:
                socket.write('Syntax: <open|process|add> (item)\n')
                break
        }
    })
})

// Start the server (on port 8125)
server.listen(8125, () => {
    console.log('server started')
})
