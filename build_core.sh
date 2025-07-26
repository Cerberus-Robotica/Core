#!/usr/bin/env bash
set -e

git submodule update --init --recursive

echo " Instalando Caronte... 🛶"

cd caronte && mkdir -p build && cd build && cmake .. && make -j4

echo " Instalando Hermes... ⚚"

cd ../../hermes && mkdir -p build && cd build && cmake .. && make -j4

echo " Instalando Hades... 🐺"

cd ../../hades && mkdir -p build && cd build && cmake .. && make -j4

cd ../..iris/web_ui/src/backend/lcm_cpp_server/ &&\
mkdir -p build && cd build && cmake .. && make

cd ../../../../../..

echo " Instalação completa!💜🐺"
