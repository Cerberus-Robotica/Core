#include "include/vision.hpp"
#include "include/socket_connect.hpp"
#include <iostream>
#include <unordered_set>

data::vision my_vision_data;
//int sock_vision;
//char buffer_vision[BUFFER_SIZE];

void recebe_dados_vision() {

    lcm::LCM lcm;

    SSL_WrapperPacket vision;
    while(true){

        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);       
        //int blue_current_size = 0;
        //int yellow_current_size = 0;
        //my_data.robots_blue.clear();
        //my_data.robots_yellow.clear();
        std::cout << "Conectando a visao..." << std::endl;
        for(int i = 0; i < 4 ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
            
            int bytes_received_vision = recvfrom(sock_vision, buffer_vision, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
            //std::cout << "Conectado a visao!!!" << std::endl;
            //std::cout << "Recebi dados da visao: " << bytes_received_vision << std::endl;
            //vision.ParseFromArray(buffer_vision, bytes_received_vision);
            //SSL_DetectionFrame detection = vision.detection();
            if (bytes_received_vision > 0) {
                // Parse dos dados recebidos (Vision)
                vision.ParseFromArray(buffer_vision, bytes_received_vision);

                
                if(vision.has_detection()){
                    my_vision_data.timestamp = vision.detection().frame_number();
                    SSL_DetectionFrame detection = vision.detection();

                    if (detection.robots_blue_size() > 0) {
                        std::unordered_set<int> blue_ids; // Conjunto para armazenar IDs únicos
                    	blue_ids.clear();
                        // Preencher o conjunto com os IDs já presentes em my_data.robots_blue
                        //for (const auto& robot : my_vision_data.robots_blue) {
                        //    blue_ids.insert(robot.robot_id);
                        //}
                    
                        for (int i = 0; i < detection.robots_blue_size(); i++) {
                            int id = detection.robots_blue(i).robot_id();
                    
                            // Se o ID já estiver presente, não adiciona de novo
                            if (blue_ids.find(id) == blue_ids.end()) {
                                data::detection_robots new_robot;
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
                        std::unordered_set<int> yellow_ids;
                    	yellow_ids.clear();
                        //for (const auto& robot : my_vision_data.robots_yellow) {
                        //    yellow_ids.insert(robot.robot_id);
                        //}
                    
                        for (int i = 0; i < detection.robots_yellow_size(); i++) {
                            int id = detection.robots_yellow(i).robot_id();
                    
                            if (yellow_ids.find(id) == yellow_ids.end()) {
                                data::detection_robots new_robot;
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
                    
                    SSL_GeometryData geometry = vision.geometry();
                    SSL_GeometryFieldSize field = geometry.field();
                    
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

	my_vision_data.robots_blue.clear();
	my_vision_data.robots_yellow.clear();
        std::cout << "field length: " << my_vision_data.field.field_length << std::endl;    
        std::cout << "\nball position_x " << my_vision_data.balls.position_x << std::endl;
        std::cout << "Robos azuis: " << my_vision_data.robots_blue_size;
        std::cout << "  Robos amarelos: " << my_vision_data.robots_yellow_size << std::endl;
        std::cout << "Timestamp: " << my_vision_data.timestamp << std::endl;
        lcm.publish("vision", &my_vision_data);
    }  
}
