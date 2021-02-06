FROM node:lts-alpine

RUN npm install -g http-server

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY package*.json ./
RUN npm install

COPY . .

RUN npm run build && npm run twbuild
RUN npm run build

EXPOSE 8080
CMD [ "http-server", "dist" ]
