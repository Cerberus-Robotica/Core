#!/usr/bin/env bash

sudo apt-get update

echo " Instalando Dependências 🐺 "

sudo apt install -qq liblcm-dev \
g++ cmake \
protobuf-compiler libprotobuf-dev \
libsdl2-dev

cd iris/web_ui && yarn add -D vite
