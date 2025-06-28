#!/bin/bash

# Entra na pasta do front-end
cd "$(dirname "$0")/web_ui" || exit

# Executa o servidor Flask em background
python3 src/backend/lcm_flask_server.py &

# Salva o PID do Flask se quiser controlar depois (opcional)
FLASK_PID=$!

# Executa o servidor dev do front-end (bloqueia até finalizar)
yarn dev

# Quando yarn dev terminar, pode opcionalmente matar o Flask (caso queira)
kill $FLASK_PID
