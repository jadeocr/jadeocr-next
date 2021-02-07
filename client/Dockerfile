# build stage
FROM node:lts-alpine as build-stage

RUN mkdir -p /app
WORKDIR /app

COPY package*.json ./
RUN npm install

COPY . .

ENV NODE_ENV="production"
ENV VUE_APP_API_BASEURL="https://next.jadeocr.com/api"

RUN npm run twbuild && npm run build

# production stage
FROM nginx:stable-alpine as production-stage
RUN mkdir -p /app

COPY --from=build-stage /app/dist /app
COPY nginx.conf /etc/nginx/nginx.conf

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
