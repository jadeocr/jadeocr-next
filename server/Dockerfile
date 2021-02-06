FROM node:14.15.4-buster-slim

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY package*.json ./
RUN npm install
RUN npm install --only=prod --silent

ENV NODE_ENV=production

COPY . .

RUN apt-get update
RUN apt-get install python3 python3-pip git -y
RUN pip3 install --upgrade pip
RUN pip3 install -r requirements.txt

RUN ["npm", "run", "build"]
EXPOSE 3000

CMD ["npm", "start"]

