echo "Iniciando Iris e Iris Server"

gnome-terminal --tab --title="Iris Web" -- bash -c "cd web_ui && yarn dev; exec bash"
gnome-terminal --tab --title="Iris Server" -- bash -c "cd web_ui/src/backend/lcm_cpp_server/build && ./server; exec bash"
