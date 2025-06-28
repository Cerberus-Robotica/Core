#!/bin/bash

# Entra na pasta do front-end
cd "$(dirname "$0")/web_ui"

# Executa o servidor dev
yarn dev
