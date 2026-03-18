#!/usr/bin/env bash

mkdir -p ./build
gcc ./src/main.c -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ./build/main
