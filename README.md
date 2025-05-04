# Core

## Instruções para instalação

### Primeiro(Instalação):

Não utilize os softwares da SSL, como o [ssl-vision](https://github.com/RoboCup-SSL/ssl-vision) e o [game-controller](https://github.com/RoboCup-SSL/ssl-game-controller), instalados localmente, utilize Docker para rodar os softwares da competição(comandos em [init_softwares_competicao.txt](https://github.com/CerberusRobotica/Core?tab=readme-ov-file#rodar-os-softwares-da-ssl));

Tutorial de instalação da bilioteca Docker e de todos os softwares da SSL utilizados nesse framework em "Programação/Frameworks da SSL/Tutorial para instalar os softwares da SSL" no drive da equipe Cerberus.
	
	
Instalar LCM(c++):

	sudo apt install liblcm-dev
		
Caso enfrente problemas na hora de buildar o software relacionado à biblioteca LCM, dê uma olhada em "Programação/Frameworks da SSL/Erros de software e biblioteca".


### Segundo(construir o framework):

Construa os três programas em C++ (com o terminal aberto na pasta principal:

	chmod +x install_core.sh && ./install_core.sh



### Finalmente(rodar):

Rodar o executável em python (com o terminal aberto na pasta principal):
	
	/bin/python3 ./iris/src/main.py

Rodar os executáveis (com o terminal aberto na pasta principal):

Hades:

	./hades/build/hades
Hermes:

	./hermes/build/hermes
Caronte:

	./caronte/build/caronte

Ou dar dois clicks nos executáveis nas pastas build/.
	
OBS: Rodar o main.py pelo VS code diminui em 15Hz a taxa de atualização do software em comparação com o terminal do linux, além de que o VS code consome uma RAM desgraçada.

## Rodar os softwares da SSL

### [Grsim](https://github.com/RoboCup-SSL/grSim):

Com VNC(Remmina): 
	
	sudo docker pull robocupssl/grsim:latest && sudo docker run --net=host -eVNC_PASSWORD=vnc -eVNC_GEOMETRY=1920x1080 robocupssl/grsim vnc

Inicie o Remmina. Conecte em localhost:5900. Insira a senha "vnc".

Ou

Headless: 
	
	sudo docker pull robocupssl/grsim:latest && sudo docker run robocupssl/grsim


### Game-Controller:

	sudo docker pull robocupssl/ssl-game-controller && sudo docker run -p 8081:8081 robocupssl/ssl-game-controller -address :8081

### AutoReferee da [Tigers](https://github.com/TIGERs-Mannheim/AutoReferee):

	./run.sh

### LCM spy:

	lcm-spy

