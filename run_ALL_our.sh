#!/usr/bin/env bash

echo "Iniciando Hades, Caronte, Hermes, Íris, GrSim, Game-Controller e Lcm_Spy"

gnome-terminal --tab --title="Hades" -- bash -c "cd hades/build && ./hades; exec bash"
gnome-terminal --tab --title="Caronte" -- bash -c "cd caronte/build && ./caronte; exec bash"
gnome-terminal --tab --title="Hermes" -- bash -c "cd hermes/build && ./hermes; exec bash"
gnome-terminal --tab --title="Íris server" -- bash -c "cd iris/web_ui && yarn dev; exec bash"
gnome-terminal --tab --title="Íris web" -- bash -c "cd iris/web_ui/src/backend/lcm_cpp_server/build && ./server; exec bash"
gnome-terminal --tab --title="Lcm-Spy" -- bash -c "lcm-spy; exec bash"
