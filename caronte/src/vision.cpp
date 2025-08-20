#include "include/vision.hpp"
#include "include/socket_connect.hpp"
#include "include/tartarus.hpp"
#include <iostream>
#include <unordered_set>
#include <thread>

data::vision my_vision_data;

//thread para receber os dados da visão e publicar no tópico "vision", pelo lcm
void recebe_dados_vision() {

    lcm::LCM lcm;

    SSL_WrapperPacket vision;
    
    data::detection_robots new_robot;

    std::unordered_set<int> yellow_ids;
    std::unordered_set<int> blue_ids; // Conjunto para armazenar IDs únicos, de robôs

    SSL_GeometryData geometry;
    SSL_GeometryFieldSize field;
    SSL_DetectionFrame detection;

    bool ssl_vision_atual;
    int cameras;

    std::cout << "Connecting to sslvision..." << std::endl;
    while(true){
        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);       
        
        if(ssl_vision_atual != han.new_tartarus.ssl_vision) {
            close(sock_vision);
            setupVisionSocket();
            ssl_vision_atual = han.new_tartarus.ssl_vision;
        }
        cameras = han.new_tartarus.ssl_vision ? 1 : 4; // 4 cameras para o grsim e 1 camera para o ssl-vision

        for(int i = 0; i < cameras ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
            
            int bytes_received_vision = recvfrom(sock_vision, buffer_vision, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);

            if (bytes_received_vision > 0) {
                // Parse dos dados recebidos (Vision)
                vision.ParseFromArray(buffer_vision, bytes_received_vision);

                if(vision.has_detection()){
                    my_vision_data.timestamp = vision.detection().frame_number();
                    detection = vision.detection();

                    if (detection.robots_blue_size() > 0) {
                        for (int i = 0; i < detection.robots_blue_size(); i++) {
                            int id = detection.robots_blue(i).robot_id();

                            my_vision_data.robots_blue[i].robot_id = id;
                            my_vision_data.robots_blue[i].position_x = detection.robots_blue(i).x();
                            my_vision_data.robots_blue[i].position_y = detection.robots_blue(i).y();
                            my_vision_data.robots_blue[i].orientation = detection.robots_blue(i).orientation();
                        }
                        
                        my_vision_data.robots_blue_size = detection.robots_blue_size(); // Atualizar tamanho real
                    }
                    else {
                        for (int i = 0; i < 16; i++) {

                            my_vision_data.robots_blue[i].robot_id = 0;
                            my_vision_data.robots_blue[i].position_x = 0;
                            my_vision_data.robots_blue[i].position_y = 0;
                            my_vision_data.robots_blue[i].orientation = 0;
                        }
                        my_vision_data.robots_blue_size = detection.robots_blue_size();
                    }
                    
                    // Para os robôs amarelos, repita o mesmo processo
                    if (detection.robots_yellow_size() > 0) {
                        for (int i = 0; i < detection.robots_yellow_size(); i++) {
                            int id = detection.robots_yellow(i).robot_id();

                            my_vision_data.robots_yellow[i].robot_id = id;
                            my_vision_data.robots_yellow[i].position_x = detection.robots_yellow(i).x();
                            my_vision_data.robots_yellow[i].position_y = detection.robots_yellow(i).y();
                            my_vision_data.robots_yellow[i].orientation = detection.robots_yellow(i).orientation();
                        }
                    
                        my_vision_data.robots_yellow_size = detection.robots_yellow_size();
                    }
                    else {
                        for (int i = 0; i < 16; i++) {

                            my_vision_data.robots_yellow[i].robot_id = 0;
                            my_vision_data.robots_yellow[i].position_x = 0;
                            my_vision_data.robots_yellow[i].position_y = 0;
                            my_vision_data.robots_yellow[i].orientation = 0;
                        }
                        my_vision_data.robots_yellow_size = detection.robots_yellow_size();
                    }

                    
                    if (vision.detection().balls_size() > 0) {
                        my_vision_data.balls.position_x = detection.balls(0).x();
                        my_vision_data.balls.position_y = detection.balls(0).y();
                    }
                
                }

                if (vision.has_geometry()) {
                    geometry = vision.geometry();
                    field = geometry.field();

                    my_vision_data.field.field_length = field.field_length();
                    my_vision_data.field.field_width = field.field_width();
                    my_vision_data.field.goal_width = field.goal_width();
                    my_vision_data.field.goal_depth = field.goal_depth();
                    my_vision_data.field.boundary_width = field.boundary_width();
                    my_vision_data.field.center_circle_radius = field.center_circle_radius();
                    my_vision_data.field.defense_area_width = field.penalty_area_width();
                    my_vision_data.field.defense_area_height = field.penalty_area_depth();
                    my_vision_data.field.line_thickness = field.line_thickness();
                    my_vision_data.field.goal_center_to_penalty_mark = field.goal_center_to_penalty_mark();
                    my_vision_data.field.goal_height = field.goal_height();
                    my_vision_data.field.ball_radius = field.ball_radius();
                    my_vision_data.field.max_robot_radius = field.max_robot_radius();
                }
            }
        }

        for(int i = 0; i < my_vision_data.robots_blue_size; i++){
            std::cout << "Robô azul ID: " << my_vision_data.robots_blue[i].robot_id << std::endl;
            std::cout << "Posição X: " << my_vision_data.robots_blue[i].position_x << std::endl;
            std::cout << "Posição Y: " << my_vision_data.robots_blue[i].position_y << std::endl;
            std::cout << "Orientação: " << my_vision_data.robots_blue[i].orientation << "\n" << std::endl;
        }

        for(int i = 0; i < my_vision_data.robots_yellow_size; i++){
            std::cout << "Robô amarelo ID: " << my_vision_data.robots_yellow[i].robot_id << std::endl;
            std::cout << "Posição X: " << my_vision_data.robots_yellow[i].position_x << std::endl;
            std::cout << "Posição Y: " << my_vision_data.robots_yellow[i].position_y << std::endl;
            std::cout << "Orientação: " << my_vision_data.robots_yellow[i].orientation << "\n" << std::endl;
        }

        //std::cout << "field length: " << my_vision_data.field.field_length << std::endl;    
        std::cout << "\nball position_x " << my_vision_data.balls.position_x << std::endl;
        std::cout << "Robos azuis: " << my_vision_data.robots_blue_size;
        std::cout << "  Robos amarelos: " << my_vision_data.robots_yellow_size << std::endl;
        std::cout << "Timestamp: " << my_vision_data.timestamp << std::endl;

        // Publica os dados no tópico "vision"
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
        //envia apenas dados de geometria de campo, alternando entre o sslvision e o grsim
        lcm.publish("vision", &my_vision_data);

        yellow_ids.clear();
	    blue_ids.clear();
    }  
}
