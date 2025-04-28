#include "esp.hpp"

// Função para enviar o Robo via serial
void esp::envia_robo() {
    //esp::Robo(1.0f, 2.0f, 1.2f);

    // Trocar "/dev/ttyUSB0" pela sua porta correta
    int serial_fd = esp_instance.abrirSerial("/dev/ttyUSB0");
    if (serial_fd == -1) {
        return;
    }
    while (true) {
        // Serializar os dados
        char data[12];
        std::memcpy(data, &esp_instance.Vx, 4);
        std::memcpy(data + 4, &esp_instance.Vy, 4);
        std::memcpy(data + 8, &esp_instance.ang, 4);

        ssize_t bytesWritten = write(serial_fd, data, sizeof(data));

        std::cout << "Dados enviados via serial!" << std::endl;

        // Esperar meio segundo
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    close(serial_fd);
}
