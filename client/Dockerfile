# build stage
FROM node:lts-alpine3.12 as build-stage

ARG API_BASEURL
RUN echo ${API_BASEURL}

RUN mkdir -p /app
WORKDIR /app

COPY package*.json ./
COPY yarn.lock ./
RUN npm install yarn
RUN yarn

COPY . .

ENV NODE_ENV="production"
# ENV NODE_OPTIONS=--openssl-legacy-provider
ENV VUE_APP_API_BASEURL=${API_BASEURL}

RUN yarn twbuild && yarn build

# production stage
FROM nginx:stable-alpine as production-stage
RUN mkdir -p /app

COPY --from=build-stage /app/dist /app
COPY nginx.conf /etc/nginx/nginx.conf

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
