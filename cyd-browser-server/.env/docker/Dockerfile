FROM node:22.1.0
RUN mkdir -p /server
RUN chown node:node /server
WORKDIR /server

USER node

RUN npm i
    
ENTRYPOINT ["npm","run","dev"]