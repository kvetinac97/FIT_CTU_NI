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
            resolve({role: "admin"})
        })
    },

    remove: (username) => {
        return new Promise((resolve, reject) => {
            resolve({removed: true})
        })
    }
}

// adding async-await
const action = async (username, password) => {
    try {
        await db.checkCredentials(username, password);
        await db.isAdmin(username);
        const status = await db.remove(username);
        console.log(status);
    } catch (error){
        console.log(error);
    }
}

action("user", "pass");
action("user", "passx");
