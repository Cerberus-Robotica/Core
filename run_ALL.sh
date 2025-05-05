#!/usr/bin/env bash

echo "Iniciando Hades, Caronte, Hermes, Íris, GrSim, Game-Controller e Lcm_Spy"

gnome-terminal --tab --title="Hades" -- bash -c "cd hades/build && ./hades; exec bash"
gnome-terminal --tab --title="Caronte" -- bash -c "cd caronte/build && ./caronte; exec bash"
gnome-terminal --tab --title="Hermes" -- bash -c "cd hermes/build && ./hermes; exec bash"
gnome-terminal --tab --title="Íris" -- bash -c "cd iris/src && python3 main.py; exec bash"
gnome-terminal --title="GrSim" -- bash -c "sudo docker pull robocupssl/grsim:latest && sudo docker run --net=host -eVNC_PASSWORD=vnc -eVNC_GEOMETRY=1920x1080 robocupssl/grsim vnc; exec bash"
gnome-terminal --title="Game-Controller" -- bash -c "sudo docker pull robocupssl/ssl-game-controller && sudo docker run -p 8081:8081 robocupssl/ssl-game-controller -address :8081; exec bash"
gnome-terminal --tab --title="Lcm-Spy" -- bash -c "lcm-spy; exec bash"
