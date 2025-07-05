#include "stm.hpp"
#include "handler.hpp"

void stm() {
    pct.id = 0;
    pct.Vx = 0;
    pct.Vy = 0;
    pct.Vang = 0;
    pct.kicker = 1000;
    int serial_port = -1;
    const char* port = "/dev/ttyUSB0"; // Altere conforme necessário
    while(serial_port < 0) {
        // Tenta abrir a porta serial até conseguir
        //std::cout << "Tentando abrir a porta serial: " << port << "..." << std::endl;
        sleep(1); // Espera 1 segundo antes de tentar novamente
        serial_port = open(port, O_RDWR);
        //std::cout << "ID atual: " << (int)pct.id << std::endl;
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
    uint8_t start[2] = {0xAA, 0xBB};
    uint8_t msg[sizeof(Pacote) + 2];
    msg[0] = start[0];
    msg[1] = start[1];

    while (true) {
            memcpy(&msg[2], &pct, sizeof(Pacote));
            write(serial_port, msg, sizeof(msg));
            usleep(5000);
    }

    close(serial_port);
}
