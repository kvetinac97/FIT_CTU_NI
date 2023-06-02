document.addEventListener('DOMContentLoaded', () => {
    // Find button and info field
    const button = document.getElementById("download-button")
    const info = document.getElementById("info-text")
    button.addEventListener('click', () => {
        downloadFile((text) => { info.innerText = text })
            .then(() => { info.innerText = "Finished downloading" })
            .catch(() => { info.innerText = "Error whilst downloading" })
    })
})

function downloadFile(updateText) {
    return new Promise((resolve, reject) => {
        // Register callbacks
        const request = new XMLHttpRequest()
        request.addEventListener('load', () => { resolve() }) // success
        request.addEventListener('error', () => { reject() }) // error
        request.addEventListener('progress', (information) => {
            const percent = Math.round(information.loaded * 100 / information.total)
            updateText(`Downloading (${percent} %)`) // update progress
        })

        // Open request
        request.open('GET', 'file.txt') // TODO: put large file.txt here
        updateText("Loading")

        // Send request
        request.send()
        updateText("Loaded")
    })
}
