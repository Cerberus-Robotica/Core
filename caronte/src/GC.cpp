#include "include/GC.hpp"
#include "include/socket_connect.hpp"
#include <iostream>

data::game_controller my_gc_data;

void recebe_dados_GC() {

    lcm::LCM lcm;

    Referee referee;
    std::cout << "Conectando ao GC..." << std::endl;
    while(true){
        
        // Recebendo os dados do socket GC
        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);

        int bytes_received_GC = recvfrom(sock_GC, buffer_GC, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
        
        if (bytes_received_GC > 0) {
            // Parse dos dados recebidos (GC)
            referee.ParseFromArray(buffer_GC, bytes_received_GC);

            if(referee.blue().name() == "Cerberus"){
                my_gc_data.team_blue = true;
                std::cout << "  Time azul " << std::endl;
            }else{
                my_gc_data.team_blue = false;
                std::cout << "  Time amarelo " << std::endl;
            }

            if(referee.has_designated_position()){
                my_gc_data.designated_position_x = referee.designated_position().x();
                my_gc_data.designated_position_y = referee.designated_position().y();
            }

            my_gc_data.current_command = referee.command(); // enum command
            
            my_gc_data.blue.name = referee.blue().name();
            my_gc_data.blue.score = referee.blue().score();
            my_gc_data.blue.red_cards = referee.blue().red_cards();
            my_gc_data.blue.yellow_cards = referee.blue().yellow_cards();
            my_gc_data.blue.timeouts = referee.blue().timeouts();
            my_gc_data.blue.timeout_time = referee.blue().timeout_time();
            my_gc_data.blue.goalkeeper_id = referee.blue().goalkeeper();

            my_gc_data.yellow.name = referee.yellow().name();
            my_gc_data.yellow.score = referee.yellow().score();
            my_gc_data.yellow.red_cards = referee.yellow().red_cards();
            my_gc_data.yellow.yellow_cards = referee.yellow().yellow_cards();
            my_gc_data.yellow.timeouts = referee.yellow().timeouts();
            my_gc_data.yellow.timeout_time = referee.yellow().timeout_time();
            my_gc_data.yellow.goalkeeper_id = referee.yellow().goalkeeper();
        }
        //system("clear"); 
        std::cout << "Timestamp: " << referee.packet_timestamp()/1000000 << std::endl;
        std::cout << "GOls azuis: " << my_gc_data.blue.score << "\n" << std::endl;
        std::cout << "time azul: " << my_gc_data.blue.name << std::endl;
        std::cout << "time amarelo: " << my_gc_data.yellow.name << std::endl;
        lcm.publish("GC", &my_gc_data);
    }  
}