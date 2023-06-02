FROM node:19-alpine3.16
WORKDIR /node
RUN npm install redis
COPY server/server.js server.js
CMD node server.js