#include "esp.hpp"

esp esp_instance;

// Função para abrir a comunicação serial
int esp::abrirSerial(const std::string& porta) {
    int fd = open(porta.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Erro ao abrir a porta serial: " << strerror(errno) << std::endl;
        return -1;
    }

    termios options;
    tcgetattr(fd, &options);

    // Configurar baudrate para 115200
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    options.c_cflag |= (CLOCAL | CREAD);    // Habilita recepção e ignora modem control lines
    options.c_cflag &= ~PARENB;              // Sem paridade
    options.c_cflag &= ~CSTOPB;              // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;                  // 8 bits

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Modo não canônico
    options.c_oflag &= ~OPOST;               // Saída bruta

    tcsetattr(fd, TCSANOW, &options);

    return fd;
}