#include "include/GC.hpp"
#include "include/socket_connect.hpp"
#include "include/tartarus.hpp"
#include <iostream>
#include <chrono>
#include <thread>

data::game_controller my_gc_data;

// thread que recebe os dados do Game Controller e publica os dados no tópico "GC", pelo do lcm
void recebe_dados_GC() {

    lcm::LCM lcm;

    Referee referee;
    const GameEvent *game_event = nullptr;
    bool updated = false;
    
    std::cout << "Conectando ao GC..." << std::endl;
    while(true){
        
        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);
        int bytes_received_GC = 0;
            // Recebendo os dados do socket GC
        if (!han.new_tartarus.iris_as_GC) {
            bytes_received_GC = recvfrom(sock_GC, buffer_GC, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        if (bytes_received_GC > 0) {
            // Parse dos dados recebidos (GC)
            referee.ParseFromArray(buffer_GC, bytes_received_GC);

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

            if(!han.new_tartarus.iris_as_GC){
                if(referee.game_events_size() > 0) {
                game_event = &referee.game_events(referee.game_events_size() - 1); // pega o último evento
                //std::cout << "game_event type: " << game_event->type() << std::endl;
                my_gc_data.game_event = game_event->type();
                }
                if(referee.has_designated_position()){
                    my_gc_data.has_designated_position = true;
                    my_gc_data.designated_position_x = referee.designated_position().x();
                    my_gc_data.designated_position_y = referee.designated_position().y();
                }
                else{
                    my_gc_data.has_designated_position = false;
                }
                if(referee.blue().name() == "Cerberus"){
                    my_gc_data.team_blue = true;
                }else{
                    my_gc_data.team_blue = false;
                }
                my_gc_data.current_command = referee.command(); // enum command
            }
        }
        if(han.new_tartarus.iris_as_GC){
            if(han.updated_tartarus != updated){
                updated = han.updated_tartarus;
                
                my_gc_data.team_blue = han.new_tartarus.team_blue;
                if(han.new_tartarus.team_blue){
                    my_gc_data.blue.goalkeeper_id = han.new_tartarus.goalkeeper_id;
                }
                else{
                    my_gc_data.yellow.goalkeeper_id = han.new_tartarus.goalkeeper_id;
                }
                my_gc_data.current_command = han.new_tartarus.iris_gc.current_command;
                my_gc_data.game_event = han.new_tartarus.iris_gc.game_event;
                if(han.new_tartarus.iris_gc.designated_position_x != my_gc_data.designated_position_x || han.new_tartarus.iris_gc.designated_position_y != my_gc_data.designated_position_y){
                    my_gc_data.has_designated_position = true;
                    my_gc_data.designated_position_x = han.new_tartarus.iris_gc.designated_position_x;
                    my_gc_data.designated_position_y = han.new_tartarus.iris_gc.designated_position_y;
                }
                else{
                    my_gc_data.has_designated_position = false;
                }
            }
        }
        lcm.publish("GC", &my_gc_data);
        //std::cout << "iris_as_gc: " << int(han.new_tartarus.iris_as_GC) << std::endl;
        //std::cout << "Game command: " << my_gc_data.current_command << std::endl;
        //std::cout << "Game event: " << my_gc_data.game_event << std::endl;
        //std::cout << "Designated position: (" << my_gc_data.designated_position_x << ", " << my_gc_data.designated_position_y << ")" << std::endl;
        //std::cout << "tartarus Designated position: (" << han.new_tartarus.iris_gc.designated_position_x << ", " << han.new_tartarus.iris_gc.designated_position_y << ")" << std::endl;
        //std::cout << "Team blue: " << (my_gc_data.team_blue ? "true" : "false") << std::endl << std::endl;
        //std::cout << "has_designated_position: " << int(my_gc_data.has_designated_position) << std::endl;
        
    }
}