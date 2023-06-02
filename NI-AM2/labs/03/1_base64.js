let data_1 = "Hello World!";
let encodedData = Buffer.from(data_1).toString('base64')
console.log(encodedData);

let data_2 = "SGVsbG8gV29ybGQh";
let decodedData = Buffer.from(data_2, 'base64').toString('ascii');
console.log(decodedData);
