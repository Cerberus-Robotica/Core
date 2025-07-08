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

        // Captura dos botões do D-Pad
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

    if (e.type == SDL_JOYBUTTONDOWN) {
        if (e.jbutton.button == 0) { // Button A (xbox) or X (playstation)
            pct.kicker = 1000; 
        }
    if (e.type == SDL_JOYBUTTONUP) {
        if (e.jbutton.button == 0) { // Button A (xbox) or X (playstation)
            pct.kicker = 0; 
        }
    }
    }
}
