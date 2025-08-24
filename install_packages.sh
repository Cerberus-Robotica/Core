#!/usr/bin/env bash

sudo apt-get update

echo " Instalando Dependências 🐺 "

sudo apt install -qq liblcm-dev \
g++ cmake \
protobuf-compiler libprotobuf-dev \
libsdl2-dev pkg-config build-essential \
gnome-terminal cmdtest

sudo apt-get install libboost-dev

