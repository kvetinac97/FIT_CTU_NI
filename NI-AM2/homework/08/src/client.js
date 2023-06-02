// Constants
const OAUTH2_CLIENT_ID = 'FILL_CLIENT_ID'
const OAUTH2_CLIENT_SECRET = 'FILL_CLIENT_SECRET'
const OAUTH2_REDIRECT_URI = 'https://localhost:8080/code'

const show_oauth2_dialog = () => {
    console.log(window.location.href)
    window.location.href = 'https://accounts.google.com/o/oauth2/v2/auth?' +
        'scope=https%3A//www.googleapis.com/auth/drive.metadata.readonly&' +
        'access_type=offline&' +
        'include_granted_scopes=true&' +
        'response_type=code&' +
        'state=state_parameter_passthrough_value&' +
        `redirect_uri=${OAUTH2_REDIRECT_URI}&` +
        `client_id=${OAUTH2_CLIENT_ID}`
}

const fetch_access_token = async (code) => {
    let response = await fetch("https://oauth2.googleapis.com/token", {
        'method': 'POST',
        'headers': {
            'Content-Type': 'application/json'
        },
        'body': JSON.stringify({
            'code': code,
            'client_id': OAUTH2_CLIENT_ID,
            'client_secret': OAUTH2_CLIENT_SECRET,
            'redirect_uri': OAUTH2_REDIRECT_URI,
            'grant_type': 'authorization_code'
        })
    })
    return response.json()
}

const request_access_token = (code) => {
    fetch_access_token(code)
        .then((result) => {
            if (result.error || !result.access_token)
                throw result

            // Load access token into cookie
            document.cookie = `access_token=${result.access_token}`
            window.location.href = '/'
        })
        .catch((error) => {
            console.log(error)

            // Display error message
            let message = document.querySelector("#message")
            message.innerText = `Error: ${error['error_description']}`

            let button = document.querySelector("#connection")
            button.style.display = 'block'
        })
}

const get_cookies = () => {
    let result = {}
    document.cookie.split("; ").forEach((cookie) => {
        let data = cookie.split("=")
        result[data[0]] = data[1]
    })
    return result
}

const fetch_data = async (cookies) => {
    let response = await fetch(`https://www.googleapis.com/drive/v2/files?access_token=${cookies.access_token}`)
    return response.json()
}

const request_data = (cookies) => {
    fetch_data(cookies)
        .then((result) => {
            if (result.error || !result.items)
                throw result
            // Show files
            show_files(result.items)
        })
        .catch((error) => {
            console.log(error)

            // Display error message
            let message = document.querySelector("#message")
            message.innerText = `Error: ${error.error.message}`

            let button = document.querySelector("#connection")
            button.style.display = 'block'
        })
}

const show_files = (items) => {
    let table = document.querySelector("#files")
    console.log(items)
    // Push table rows for each item
    items.forEach((item) => {
        let tr = document.createElement("tr")
        let kind = item.kind.replace("drive#", "")
        let lastModifier = item['lastModifyingUserName'] ?? 'Unknown'
        let modifiedDate = new Date(item['modifiedDate']).toLocaleString()

        tr.innerHTML = `<td>${kind}</td><td>${item.title}</td><td>${lastModifier}</td>` +
                    `<td>${modifiedDate}</td>`
        table.appendChild(tr)
    })
    // Display the table and logout button
    document.querySelector("#logout").style.display = 'inline'
    document.querySelector("#files").style.display  = 'table'
}

document.addEventListener('DOMContentLoaded', () => {
    let cookies = get_cookies()
    let urlParams = new URLSearchParams(window.location.href)

    // Add listeners
    let button = document.querySelector("#connection")
    button.addEventListener('click', () => {
        show_oauth2_dialog()
    })

    let logout = document.querySelector("#logout")
    logout.addEventListener('click', () => {
        document.cookie = "access_token=" // reset cookie
        window.location.reload()
    })

    // If there is a token, try to request data
    if (cookies.access_token)
        return request_data(cookies)

    // Coming from auth = get access token from API
    if (urlParams.has("code"))
        return request_access_token(urlParams.get("code"))

    // No token = show auth button
    button.style.display = 'block'
})
