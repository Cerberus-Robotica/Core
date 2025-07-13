#include "manual_control.hpp"

controller control_obj;

void controller::connect_controller() {
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL não pôde ser inicializado: " << SDL_GetError() << std::endl;
        return;
    }

    if (SDL_NumJoysticks() < 1) {
        std::cerr << "Nenhum controle conectado.\n";
    } else {
        joy = SDL_JoystickOpen(0);
        if (joy == nullptr) {
            std::cerr << "Erro ao abrir joystick: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Controle conectado: " << SDL_JoystickName(joy) << std::endl;
        }
    }
}

void controller::control() {
    SDL_PollEvent(&e);
    if (e.type == SDL_JOYAXISMOTION) {
        switch (e.jaxis.axis) {
            case 0: // Analógico esquerdo - eixo X
                pct.Vy = -normalizeAxis(e.jaxis.value); // Direita = +1.0 (invertido)
                break;
            case 1: // Analógico esquerdo - eixo Y
                pct.Vx = -normalizeAxis(e.jaxis.value); // Cima = +1.0 
                break;
            case 3: // Analógico direito - eixo X
                pct.Vang = -normalizeAxis(e.jaxis.value); // Direita = -1.0
                break;
            default:
                break;
        }
    }

        // Captura dos botões do D-Pad (XBOX)
    if (e.type == SDL_JOYHATMOTION) {
        if (e.jhat.hat == 0) { // geralmente 0 é o D-Pad
            uint8_t dir = e.jhat.value;

            if (dir & SDL_HAT_UP) {
                control_obj.robot_id = 0;

            } else if (dir & SDL_HAT_DOWN) {
                control_obj.robot_id = 2;

            } else if (dir & SDL_HAT_RIGHT) {
                control_obj.robot_id = 1;

            } else if (dir & SDL_HAT_LEFT) {
                control_obj.robot_id = 3;

            }
        }
    }

    if (e.type == SDL_JOYBUTTONDOWN) { // suporte para controle de ps3
        switch (e.jbutton.button) {
            case 13: // Exemplo: D-Pad cima
                control_obj.robot_id = 0;
                break;
            case 14: // D-Pad direita
                control_obj.robot_id = 1;
                break;
            case 15: // D-Pad baixo
                control_obj.robot_id = 5;
                break;
            case 16: // D-Pad esquerda
                control_obj.robot_id = 4;
                break;
        
        }
    }

    if (e.type == SDL_JOYBUTTONDOWN) {
        switch(e.jbutton.button) { 
            case 0:    // Button A (xbox) or X (playstation)
                pct.kicker = 1000; 
                break;
            case 4:    // Button LB (xbox) or L1 (playstation)
                control_obj.robot_id--; // Decrementa o ID do robô
                break;
            case 5:    // Button RB (xbox) or R1 (playstation)
                control_obj.robot_id++; // Incrementa o ID do robô
                break;
        }
    }

    if (e.type == SDL_JOYBUTTONUP) {
        if (e.jbutton.button == 0) { // Button A (xbox) or X (playstation)
            pct.kicker = 0; 
        }
    }
    
    if(control_obj.robot_id < 0) {
        control_obj.robot_id = 15; // Garante que o ID do robô não seja negativo
    }
    if(control_obj.robot_id > 15) {
        control_obj.robot_id = 0; // Garante que o ID do robô não ultrapasse o limite
    }
}
