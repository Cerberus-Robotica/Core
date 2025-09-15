#include "include/vision_master.hpp"
#include "include/tartarus.hpp"

data::vision my_vision_data;

//thread para receber os dados da visão e publicar no tópico "vision", pelo lcm
void vision_master::recebe_dados_vision() {

    SSL_WrapperPacket vision;
    
    data::detection_robots new_robot;

    SSL_GeometryData geometry;
    SSL_GeometryFieldSize field;
    SSL_DetectionFrame detection;

    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);       

    for(int i = 0; i < vision_master_instance.cameras ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
        //if(han.updated_tartarus == vision_master_instance.updated) {
        //    break; // Sai do loop se tartarus tem atualizações
        //}
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
                        if(my_vision_data.robots_blue[id].detected == false) {
                            my_vision_data.robots_blue[id].detected = true;
                            my_vision_data.robots_blue[id].robot_id = id;
                            my_vision_data.robots_blue[id].position_x = detection.robots_blue(i).x();
                            my_vision_data.robots_blue[id].position_y = detection.robots_blue(i).y();
                            my_vision_data.robots_blue[id].orientation = detection.robots_blue(i).orientation();
                            vision_master_instance.blue_ids.insert(id);
                        }
                    }
                }
                    
                // Para os robôs amarelos, repita o mesmo processo
                if (detection.robots_yellow_size() > 0) {
                    for (int i = 0; i < detection.robots_yellow_size(); i++) {
                        int id = detection.robots_yellow(i).robot_id();
                        if(my_vision_data.robots_yellow[id].detected == false) {
                            my_vision_data.robots_yellow[id].detected = true;
                            my_vision_data.robots_yellow[id].robot_id = id;
                            my_vision_data.robots_yellow[id].position_x = detection.robots_yellow(i).x();
                            my_vision_data.robots_yellow[id].position_y = detection.robots_yellow(i).y();
                            my_vision_data.robots_yellow[id].orientation = detection.robots_yellow(i).orientation();
                            vision_master_instance.yellow_ids.insert(id);
                        }
                    }
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

                if(han.new_tartarus.ssl_vision == false){
                    my_vision_data.field.center_circle_radius = 500.0;
                    my_vision_data.field.line_thickness = 10.0; 
                    my_vision_data.field.goal_height = 155.0;
                    my_vision_data.field.ball_radius = 21.5;
                    my_vision_data.field.max_robot_radius = 90.0;
                    if(my_vision_data.field.field_length == 4500){ //div EL
                        my_vision_data.field.goal_center_to_penalty_mark = 3000.0;
                        my_vision_data.field.defense_area_width = 1350.0;
                        my_vision_data.field.defense_area_height = 500.0;
                    }
                    else if(my_vision_data.field.field_length == 12000){ //div A
                        my_vision_data.field.goal_center_to_penalty_mark = 8000.0;
                        my_vision_data.field.defense_area_width = 3600.0;
                        my_vision_data.field.defense_area_height = 1800.0;

                    }

                    
                }
            }
        }
    }
    my_vision_data.robots_yellow_size = vision_master_instance.yellow_ids.size();
    my_vision_data.robots_blue_size = vision_master_instance.blue_ids.size();
    // Print field geometry values
    //std::cout << "field.field_length: " << my_vision_data.field.field_length << '\n'
    //          << "field.field_width: " << my_vision_data.field.field_width << '\n'
    //          << "field.goal_width: " << my_vision_data.field.goal_width << '\n'
    //          << "field.goal_depth: " << my_vision_data.field.goal_depth << '\n'
    //          << "field.boundary_width: " << my_vision_data.field.boundary_width << '\n'
    //          << "field.center_circle_radius: " << my_vision_data.field.center_circle_radius << '\n'
    //          << "field.defense_area_width: " << my_vision_data.field.defense_area_width << '\n'
    //          << "field.defense_area_height: " << my_vision_data.field.defense_area_height << '\n'
    //          << "field.line_thickness: " << my_vision_data.field.line_thickness << '\n'
    //          << "field.goal_center_to_penalty_mark: " << my_vision_data.field.goal_center_to_penalty_mark << '\n'
    //          << "field.goal_height: " << my_vision_data.field.goal_height << '\n'
    //          << "field.ball_radius: " << my_vision_data.field.ball_radius << '\n'
    //          << "field.max_robot_radius: " << my_vision_data.field.max_robot_radius << '\n'<<std::endl;
    // Publica os dados no tópico "vision"
    //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    //envia apenas dados de geometria de campo, alternando entre o sslvision e o grsim
    //lcm.publish("vision", &my_vision_data);
}  

