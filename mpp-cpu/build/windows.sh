#!/bin/bash

GOOS=windows GOARCH=x86_64 CGO_ENABLED=1 CC=x86_64-w64-mingw32-gcc make