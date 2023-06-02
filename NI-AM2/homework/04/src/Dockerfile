FROM node:19-alpine3.16
WORKDIR /server
RUN npm install redis
COPY server.js .
EXPOSE 8888
CMD node server.js