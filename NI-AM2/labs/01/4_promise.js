const db = {
    checkCredentials: (username, password) => {
        return new Promise((resolve, reject) => {
            if (username === "user" && password === "pass")
                resolve({verified: true})
            else
                reject("invalid credentials")
        })
    },

    isAdmin: (username) => {
        return new Promise((resolve, reject) => {
            resolve({role: "admin"});
        })
    },

    remove: (username) => {
        return new Promise((resolve, reject) => {
            resolve({removed: true});
        })
    }
}

// promise is much easier and better
const action = (username, password) => {
    db.checkCredentials(username, password)
        .then(() => {return db.isAdmin(username)})
        .then(() => {return db.remove(username)})
        .then((status) => {
            console.log(status)
        })
        .catch((error) => {
            console.log(error)
        })
}

action("user", "pass")
action("user", "passx")
