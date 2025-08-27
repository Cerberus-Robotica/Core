#include "stm.hpp"

void stm::stm_connect() { // function to send data to the robots via STM32
    stm_obj.serial_port = -1;

    while(stm_obj.serial_port < 0) {
        // Tenta abrir a porta serial até conseguir
        std::cout << "Tentando abrir a porta serial: " << port << han.data_tartarus_copy.stm_port << "..." << std::endl;
        sleep(1); // Espera 1 segundo antes de tentar novamente
        std::string porta_usb = "/dev/ttyUSB" + std::to_string(han.data_tartarus_copy.stm_port);
        stm_obj.serial_port = open(porta_usb.c_str(), O_RDWR);
        //std::cout << "ID atual: " << (int)pct.id << std::endl;
        if(han.data_tartarus_copy.ssl_vision == 0){
            break;
        }
    }

    if (stm_obj.serial_port < 0) {
        std::cerr << "Erro ao abrir a porta serial " << port << std::endl;
    }

    // Configurações da UART
    termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(stm_obj.serial_port, &tty) != 0) {
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

    tcsetattr(stm_obj.serial_port, TCSANOW, &tty);


    // Envio de mensagem
    stm_obj.msg[0] = start[0];
    stm_obj.msg[1] = start[1];
}