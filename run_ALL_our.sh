  GNU nano 6.2                       run_ALL.sh                                 
#!/usr/bin/env bash

echo "Iniciando Hades, Caronte, Hermes, Íris, GrSim, Game-Controller e Lcm_Spy"

gnome-terminal --tab --title="Hades" -- bash -c "cd hades/build && ./hades; exe>
gnome-terminal --tab --title="Caronte" -- bash -c "cd caronte/build && ./caront>
gnome-terminal --tab --title="Hermes" -- bash -c "cd hermes/build && ./hermes; >
gnome-terminal --tab --title="Íris" -- bash -c "cd iris/src && python3 main.py;>
gnome-terminal --tab --title="Lcm-Spy" -- bash -c "lcm-spy; exec bash"
