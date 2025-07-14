#!/usr/bin/env bash
set -e



echo " Instalando Caronte... 🛶, Hermes... ⚚, Hades... 🐺"

mkdir -p build && cd build && cmake .. && make -j4

cd ..

echo " Instalação completa!💜🐺"
