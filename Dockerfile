FROM --platform=amd64 emscripten/emsdk:latest AS build-core

RUN mkdir -p /mpp-cpu
WORKDIR /mpp-cpu

COPY mpp-cpu .

RUN make



FROM node:alpine AS install-deps

WORKDIR /app
COPY mpp-front/package.json package.json
COPY mpp-front/yarn.lock yarn.lock
RUN yarn


FROM node:alpine AS build

ENV GENERATE_SOURCEMAP=false

WORKDIR /app

COPY mpp-front .
COPY --from=install-deps /app/node_modules /app/node_modules
COPY --from=build-core /mpp-cpu/output/. src/lib/core/files

RUN yarn build

