#!/usr/bin/env bash
set -e

git submodule update --init --recursive

echo " Instalando Caronte... 🛶, Hermes... ⚚, Hades... 🐺"

mkdir -p build && cd build && cmake .. && make -j4

cd ..

echo " Instalação completa!💜🐺"
