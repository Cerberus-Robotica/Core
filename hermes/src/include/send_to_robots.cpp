#include "send_to_robots.hpp"

bool time_atual;
Pacote pct;

void robots_sender::send_to_grsim() { // function to send data to grSim
    commands.Clear();
    std::cout << "team robot size " << han.data_ia_copy.robots_size << std::endl;
    std::cout << "timestamp " << han.data_ia_copy.timestamp << std::endl;

    if (han.data_ia_copy.robots_size == 0) {
        std::cerr << "Warning: robots_size is 0, skipping send_to_grsim()" << std::endl;
        return;
    }

    if(han.data_tartarus_copy.team_blue == time_atual)
    {
        for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
            const data::robot * robot_ia = &han.data_ia_copy.robots[i];
            auto robotpacket = commands.add_robot_commands();
            robotpacket->set_id(robot_ia->id);
            robotpacket->set_wheelsspeed(robot_ia->wheel_speed);
            robotpacket->set_kickspeedx(robot_ia->kick_speed_x);
            robotpacket->set_kickspeedz(robot_ia->kick_speed_z);
            robotpacket->set_veltangent(robot_ia->vel_tang);
            robotpacket->set_velnormal(robot_ia->vel_normal);
            robotpacket->set_velangular(robot_ia->vel_ang);
            robotpacket->set_spinner(robot_ia->spinner);
            robotpacket->set_wheel1(robot_ia->wheel_fl);//certo
            robotpacket->set_wheel2(robot_ia->wheel_bl);
            robotpacket->set_wheel3(robot_ia->wheel_br);
            robotpacket->set_wheel4(robot_ia->wheel_fr);//certo

        }
        commands.set_timestamp(han.data_ia_copy.timestamp);
        commands.set_isteamyellow(!han.data_tartarus_copy.team_blue); 
    }
    else{
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
            const data::robot * robot_ia = &han.data_ia_copy.robots[i];
            auto robotpacket = commands.add_robot_commands();
            robotpacket->set_id(robot_ia->id);
            robotpacket->set_wheelsspeed(true); //bool
            robotpacket->set_kickspeedx(robot_ia->kick_speed_x);
            robotpacket->set_kickspeedz(robot_ia->kick_speed_z);
            robotpacket->set_veltangent(robot_ia->vel_tang);
            robotpacket->set_velnormal(robot_ia->vel_normal);
            robotpacket->set_velangular(robot_ia->vel_ang);
            robotpacket->set_spinner(robot_ia->spinner); //bool
            robotpacket->set_wheel1(0);//certo
            robotpacket->set_wheel2(0);
            robotpacket->set_wheel3(0);
            robotpacket->set_wheel4(0);//certo

        }
        commands.set_timestamp(han.data_ia_copy.timestamp);
        commands.set_isteamyellow(han.data_tartarus_copy.team_blue);
        time_atual = han.data_tartarus_copy.team_blue;
    }




    packet_grsim.mutable_commands()->CopyFrom(commands);
    std::cout << "commands size " << packet_grsim.commands().robot_commands_size() << std::endl;

    std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
    packet_grsim.SerializeAsString();
    std::string serialized_packet = packet_grsim.SerializeAsString();
    sendto(sock_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_grsim, sizeof(addr_grsim));
    std::cout << "Sending to grSim..." << std::endl;
        

}


void robots_sender::stm_connect() { // function to send data to the robots via STM32
    pct.id = 0;
    pct.Vx = 0;
    pct.Vy = 0;
    pct.Vang = 0;
    pct.kicker = 1000;
    serial_port = -1;

    while(serial_port < 0) {
        // Tenta abrir a porta serial até conseguir
        std::cout << "Tentando abrir a porta serial: " << port << han.data_tartarus_copy.stm_port << "..." << std::endl;
        sleep(1); // Espera 1 segundo antes de tentar novamente
        std::string porta_usb = "/dev/ttyUSB" + std::to_string(han.data_tartarus_copy.stm_port);
        serial_port = open(porta_usb.c_str(), O_RDWR);
        //std::cout << "ID atual: " << (int)pct.id << std::endl;
        if(han.data_tartarus_copy.ssl_vision == 0){
            break;
        }
    }

    if (serial_port < 0) {
        std::cerr << "Erro ao abrir a porta serial " << port << std::endl;
    }

    // Configurações da UART
    termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Erro ao obter atributos da UART\n";
    }

    // Baud rate
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    tty.c_cflag &= ~PARENB; // Sem paridade
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits
    tty.c_cflag &= ~CRTSCTS;// Sem controle de fluxo
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Modo raw
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    tcsetattr(serial_port, TCSANOW, &tty);


    // Envio de mensagem
    msg[0] = start[0];
    msg[1] = start[1];
}

void robots_sender::send_control() { // global function to send control commands
    control_obj.robot_id = 0; // Default robot ID via controller

    while(true) {
        control_obj.connect_controller(); // Conecta o controle
        while(han.data_tartarus_copy.bool_controller == 1 && SDL_NumJoysticks() < 1) { //only works with UI
            control_obj.connect_controller(); // try to connect the controller
            std::cout << "trying to connect the controller..." << std::endl;
            sleep(1.0);
        }
        
        if (han.data_tartarus_copy.ssl_vision == 0) {
            close(serial_port);
            setupSocket_grsim();
            while(han.data_tartarus_copy.ssl_vision == 0) {

                if(han.data_tartarus_copy.competition_mode == 0) {
                    control_obj.control(); // Mantém atualizando

                    for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
                        data::robot* r = &han.data_ia_copy.robots[i];
                        if (r->id == control_obj.robot_id) {
                            r->vel_tang = pct.Vx;
                            r->vel_normal = pct.Vy;
                            r->vel_ang = pct.Vang;
                            std::cout << "Robot ID: " << (int)control_obj.robot_id << " Vx: " << r->vel_tang << " Vy: " << r->vel_normal << " Vang: " << r->vel_ang << std::endl;
                        }
                        else {
                            r->vel_tang = 0;
                            r->vel_normal = 0;
                            r->vel_ang = 0;
                        }
                    }
                }

                send_to_grsim();
            }
        } else {
            stm_connect();
            while(han.data_tartarus_copy.ssl_vision == 1) {
                if(han.data_tartarus_copy.competition_mode == 0) {
                    control_obj.control(); // Mantém atualizando
                }
                else {
                    pct.id = han.data_ia_copy.robots[0].id;
                    pct.Vx = han.data_ia_copy.robots[0].vel_tang; //vx é o vel_tang
                    pct.Vy = han.data_ia_copy.robots[0].vel_normal; //vy é o vel_normal
                    pct.Vang = han.data_ia_copy.robots[0].vel_ang;
                    pct.kicker = han.data_ia_copy.robots[0].kick_speed_x;
                }
                memcpy(&msg[2], &pct, sizeof(Pacote));
                write(serial_port, msg, sizeof(msg));
                usleep(5000);
            }
        }
        sleep(1); // Sleep for a while before sending again
    }
    close(serial_port);
    close(sock_grsim);
}