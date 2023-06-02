const db = {
    checkCredentials: (username, password, callback) => {
        if (username === "user" && password === "pass")
            callback(null, { verified: true })
        else callback("invalid credentials", {verified: false})
    },

    isAdmin: (username, callback) => {
        callback(null, {role: "admin"})
    },

    remove: (username, callback) => {
        callback(null, {removed: true})
    }
}

const action = (username, password, callback) => {
    db.checkCredentials(username, password, (error, verified) => {
        if (error) {
            callback(error)
            return
        }
        db.isAdmin(username, (error, status) => {
            if (error) {
                callback(error)
                return
            }
            db.remove(username, (error, status) => {
                if (error) callback(error)
                else callback(null, status)
            })
        })
    });
}

action("user", "pass", (error, status) => {
    console.log(error, status);
});

action("user", "passx", (error, status) => {
    console.log(error, status);
});
