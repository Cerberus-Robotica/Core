#include "include/vision.hpp"
#include "include/socket_connect.hpp"
#include <iostream>
#include <unordered_set>
#include <thread>

data::vision my_vision_data;
//int sock_vision;
//char buffer_vision[BUFFER_SIZE];

void recebe_dados_vision() {

    lcm::LCM lcm;

    SSL_WrapperPacket vision;
    
    data::detection_robots new_robot;

    std::unordered_set<int> yellow_ids;
    std::unordered_set<int> blue_ids; // Conjunto para armazenar IDs únicos

    SSL_GeometryData geometry;
    SSL_GeometryFieldSize field;
    SSL_DetectionFrame detection;
    
    std::cout << "Conectando a visao..." << std::endl;
    while(true){

        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);       
        
        
        for(int i = 0; i < 4 ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
            
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
                    
                            // Caso o ID seja encontrado em .find(), ele retorna um iterador que aponta para o id atual, caso contrário, retorna blue_ids.end()
                            if (blue_ids.find(id) == blue_ids.end()) {
                                
                                new_robot.robot_id = id;
                                new_robot.position_x = detection.robots_blue(i).x();
                                new_robot.position_y = detection.robots_blue(i).y();
                                new_robot.orientation = detection.robots_blue(i).orientation();
                                
                                my_vision_data.robots_blue.push_back(new_robot);
                                blue_ids.insert(id); // Adiciona o ID ao conjunto para evitar duplicatas
                            }
                        }
                        
                        my_vision_data.robots_blue_size = my_vision_data.robots_blue.size(); // Atualizar tamanho real
                    }
                    
                    // Para os robôs amarelos, repita o mesmo processo
                    if (detection.robots_yellow_size() > 0) {
                    
                        for (int i = 0; i < detection.robots_yellow_size(); i++) {
                            int id = detection.robots_yellow(i).robot_id();
                    
                            if (yellow_ids.find(id) == yellow_ids.end()) {
                                
                                new_robot.robot_id = id;
                                new_robot.position_x = detection.robots_yellow(i).x();
                                new_robot.position_y = detection.robots_yellow(i).y();
                                new_robot.orientation = detection.robots_yellow(i).orientation();
                                my_vision_data.robots_yellow.push_back(new_robot);
                                yellow_ids.insert(id);
                            }
                        }
                    
                        my_vision_data.robots_yellow_size = my_vision_data.robots_yellow.size();
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

    std::cout << "field length: " << my_vision_data.field.field_length << std::endl;    
    std::cout << "\nball position_x " << my_vision_data.balls.position_x << std::endl;
    std::cout << "Robos azuis: " << my_vision_data.robots_blue_size;
    std::cout << "  Robos amarelos: " << my_vision_data.robots_yellow_size << std::endl;
    std::cout << "Timestamp: " << my_vision_data.timestamp << std::endl;

    // Publica os dados no tópico "vision"
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    lcm.publish("vision", &my_vision_data);

    yellow_ids.clear();
	blue_ids.clear();
	my_vision_data.robots_blue.clear();
	my_vision_data.robots_yellow.clear();
    }  
}
