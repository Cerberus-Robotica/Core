# Core



```mermaid
flowchart LR
    %% --- Entradas ---
    subgraph Inputs
        direction TB
        vision("SSL Vision")
        GC("Game Controller")
        grsim("GrSim")
    end

    %% --- Núcleo ---
    subgraph Core
        direction LR
        Caronte["Caronte"] --> Hades["Hades"]
        Caronte --> Iris["Iris"]
        Hades --> Hermes["Hermes"]
        Hades --> Iris
        Iris --> Hades
        Iris --> Hermes
        Iris --> Caronte
    end

    %% --- Saídas ---
    subgraph Outputs
        direction TB
        radio[/Radio\]
        grsim_out("GrSim")
    end

    %% Conexões externas
    vision --> Caronte
    GC --> Caronte
    grsim --> Caronte

    Hermes --> grsim_out
    Hermes --> radio

```


## Instruções para instalação

### Instalação:

Não utilize o software [game-controller](https://github.com/RoboCup-SSL/ssl-game-controller) instalado localmente, porém instale as dependências dele em seu dispostivo.

Utilize Docker para rodar os softwares da competição(comandos em [Rodar os softwares da SSL](https://github.com/CerberusRobotica/Core?tab=readme-ov-file#rodar-os-softwares-da-ssl) para rodar manualmente);

Tutorial de instalação da bilioteca Docker e de todos os softwares da SSL utilizados nesse framework em "Programação/Frameworks da SSL/Tutorial para instalar os softwares da SSL" no drive da equipe Cerberus.

## Frameworks utilizados:

* [GrSim](https://github.com/RoboCup-SSL/grSim) ou [GrSim para a Entry-Level](https://github.com/Cerberus-Robotica/grSim_for_EL) (simulador)
* [SSL-Vision](https://github.com/RoboCup-SSL/ssl-vision/tree/master) (sistema de visão)
* [SSL-Game-Controller](https://github.com/RoboCup-SSL/ssl-game-controller) (controlador de jogo)
### Opcional:
* [Tigers AutoReferee](https://github.com/TIGERs-Mannheim/AutoReferee)
	
Instalar dependências do Core:

	./install_packages.sh
		

### Segundo(construir o framework):

Construa os quatro programas principais em C++ (com o terminal aberto na pasta principal):

	./build_core.sh

### Finalmente(rodar):

Rodar os executáveis (com o terminal aberto na pasta principal)

Para rodar apenas o Hades, Hermes, Caronte e Íris:

	./run.sh
 
e, em outro terminal:

	./iris_server.sh
 
ou, para rodar com o GrSim e o Game-Controller(necessário docker), apenas rode:
 
 	./run_with_simulator.sh

	
## Rodar os softwares da SSL(manualmente)

### [Grsim](https://github.com/RoboCup-SSL/grSim):

Você pode utilizar o [Grsim modificado para a SSL-EL](https://github.com/Cerberus-Robotica/grSim_for_EL) ou com docker:

Com VNC(Remmina): 
	
	sudo docker pull robocupssl/grsim:latest && sudo docker run --net=host -eVNC_PASSWORD=vnc -eVNC_GEOMETRY=1920x1080 robocupssl/grsim vnc

Inicie o Remmina. Conecte em localhost:5900. Insira a senha "vnc".

Ou

Headless: 
	
	sudo docker pull robocupssl/grsim:latest && sudo docker run robocupssl/grsim


### Game-Controller:

	sudo docker pull robocupssl/ssl-game-controller && sudo docker run -p 8081:8081 robocupssl/ssl-game-controller -address :8081

### AutoReferee da [Tigers](https://github.com/TIGERs-Mannheim/AutoReferee):

no software da tigers:

	./run.sh

### LCM spy(é necessário ter o java instalado):

	lcm-spy

## Orientação utilizada pela equipe:

![Robot Orientation](Robot_orientation.png "robot orientation")
