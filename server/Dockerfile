FROM node:12.18-alpine

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

ENV NODE_ENV=production
COPY . .

RUN npm install --only=prod --silent
RUN ["npm", "run", "build"]
EXPOSE 3000

CMD ["npm", "start"]
