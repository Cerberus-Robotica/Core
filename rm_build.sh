#!/usr/bin/env bash
set -e

echo " Removendo Caronte... 🛶"

cd caronte && rm -rf build
rm -rf .vscode
rm -rf .idea

echo " Removendo Hermes... ⚚"

cd ../hermes && rm -rf build
rm -rf .vscode
rm -rf .idea

echo " Removendo Hades... 🐺"

cd ../hades && rm -rf build
rm -rf cmake-build-debug
rm -rf .idea
rm -rf .vscode

cd ../..
rm -rf .vscode
rm -rf .idea
echo " Remoção completa!💜🐺"
