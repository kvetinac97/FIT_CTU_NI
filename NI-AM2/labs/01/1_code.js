// objects - key/value pairs
var obj = { name: "Tomas", "main-city" : "Innsbruck", value : 3 };
obj.name = "Peter"; // assign the name property another value
obj["main-city"] = "Prague"; // another way to access object's values; it's not an array!
console.log(obj)

// arrays
var a = ["Tomas", "Peter", "Alice"];
for (var i = 0; i < a.length; i++)
    console.log(JSON.stringify(a[i]))

// combinations of arrays and objects
     var obj_a = [
         { name: "Tomas", city: "Innsbruck" },
         { name : "Peter", city : "Prague" },
         { name : "Alice", cities : ["Prague", "Brno"] } ];

for (var i = 0; i < obj_a.length; i++)
    console.log(obj_a[i].name)

// assign a function to a variable
var minus = function(a, b) {
    return a - b;
}

// call the function;
// now you can pass 'minus' as a parameter to another function
var r2 = minus(6, 4);
console.log(r2)

// function returns the result through a callback, not directly;
// this is not a non-blocking I/O, just demonstration of the callback
function add(a, b, callback) {
    callback(a + b);
}

// assign the callback to a variable
var print = function(result) {
    console.log(result);
};

// call the function with callback as a parameter
add(7, 8, print);

var obj = {
    data : [2, 3, "Tomas", "Alice", 4 ],

    getIndexOf : function(val) {
        for (var i = 0; i < this.data.length; i++)
            if (this.data[i] == val)
                return i;
        return -1;
    }
}

console.log(obj.getIndexOf(3)); // will return 1

function adder() {
    sum = 0;
    return function(x) {
        sum += x;
        return sum;
    }
}

var pos = adder();

console.log(pos(3)); // returns 3
console.log(pos(4)); // returns 7
console.log(pos(5)); // returns 12

setTimeout(() => {
    console.log('timeout');
}, 0)

setImmediate(() => {
    console.log('immediate');
})
