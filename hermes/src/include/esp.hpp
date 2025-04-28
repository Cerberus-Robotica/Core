#ifndef ESP_HPP
#define ESP_HPP

#include <thread>       
#include <chrono>       
#include <cstring>     
#include <fcntl.h>     
#include <unistd.h>     
#include <termios.h>    
#include <errno.h> 
#include <iostream>
#include <string>
//#include <linux/serial.h> //ou <serial/serial.h>

class esp {
    public:
    
        float Vx;
        float Vy;
        float ang;

        void envia_robo();

        int abrirSerial(const std::string& porta);
};



extern esp esp_instance;

#endif // ESP_HPP