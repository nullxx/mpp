#!/bin/sh

rm -rf docs

npm run build --prefix _docs

mv _docs/dist docs