# FROM node:14.15.4-buster-slim
FROM tensorflow/tensorflow:latest-gpu 

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

RUN apt-get update && apt-get install python3 python3-pip git -y

ENV NODE_VERSION=14.15.0
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.3/install.sh | bash
ENV NVM_DIR=/root/.nvm
RUN . "$NVM_DIR/nvm.sh" && nvm install ${NODE_VERSION}
RUN . "$NVM_DIR/nvm.sh" && nvm use v${NODE_VERSION}
RUN . "$NVM_DIR/nvm.sh" && nvm alias default v${NODE_VERSION}
ENV PATH="/root/.nvm/versions/node/v${NODE_VERSION}/bin/:${PATH}"

COPY requirements.txt ./

RUN pip3 install --upgrade pip && pip3 install -r requirements.txt

COPY . .

RUN npm install --only=prod --silent

RUN ["npm", "run", "build"]
EXPOSE 3000

CMD ["npm", "start"]

