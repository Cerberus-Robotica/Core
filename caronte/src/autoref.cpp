#include "include/vision_master.hpp"
#include "include/tartarus.hpp"

data::autoref my_autoref_data;

void vision_master::recebe_dados_tracked() {
    TrackerWrapperPacket tracked_packet;
    TrackedFrame tracked_frame;

    data::detection_robots_tracked new_robot;

    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    for(int i = 0; i < vision_master_instance.cameras ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
        //if(han.updated_tartarus == vision_master_instance.updated) {
        //    break; // Sai do loop se tartarus tem atualizações
        //}
        int bytes_received_TRACKED = recvfrom(sock_TRACKED, buffer_TRACKED, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
        //std::cout << "bytes_received_TRACKED: " << bytes_received_TRACKED << std::endl;

        if (bytes_received_TRACKED > 0) {
            // Parse dos dados recebidos (Tracked)
            tracked_packet.ParseFromArray(buffer_TRACKED, bytes_received_TRACKED);    
            if(tracked_packet.has_tracked_frame()){
                tracked_frame = tracked_packet.tracked_frame();
                my_autoref_data.timestamp = tracked_frame.timestamp();

                if (tracked_frame.robots_size() > 0) {
                    for (int i = 0; i < tracked_frame.robots_size(); i++) {
                        RobotId new_robot_id = tracked_frame.robots(i).robot_id();
                        if(new_robot_id.team() == 2){ //UNKNOWN = 0, YELLOW = 1, BLUE = 2
                            my_vision_data.robots_blue[new_robot_id.id()].detected = true;
                            my_vision_data.robots_blue[new_robot_id.id()].robot_id = new_robot_id.id();
                            my_vision_data.robots_blue[new_robot_id.id()].position_x = tracked_frame.robots(i).pos().x()*1000; // mm
                            my_vision_data.robots_blue[new_robot_id.id()].position_y = tracked_frame.robots(i).pos().y()*1000;
                            my_autoref_data.robots_blue[new_robot_id.id()].vel_x = tracked_frame.robots(i).vel().x()*1000; // mm/s
                            my_autoref_data.robots_blue[new_robot_id.id()].vel_y = tracked_frame.robots(i).vel().y()*1000;
                            my_autoref_data.robots_blue[new_robot_id.id()].vel_angular = tracked_frame.robots(i).vel_angular(); //rad/s
                            my_vision_data.robots_blue[new_robot_id.id()].orientation = tracked_frame.robots(i).orientation();
                        }
                        
                    
                        else if(new_robot_id.team() == 1){ //UNKNOWN = 0, YELLOW = 1, BLUE = 2
                        // Para os robôs amarelos, repita o mesmo processo
                            my_vision_data.robots_yellow[new_robot_id.id()].detected = true;
                            my_vision_data.robots_yellow[new_robot_id.id()].robot_id = new_robot_id.id();
                            my_vision_data.robots_yellow[new_robot_id.id()].position_x = tracked_frame.robots(i).pos().x()*1000; // mm
                            my_vision_data.robots_yellow[new_robot_id.id()].position_y = tracked_frame.robots(i).pos().y()*1000; // mm
                            my_autoref_data.robots_yellow[new_robot_id.id()].vel_x = tracked_frame.robots(i).vel().x()*1000; // mm/s
                            my_autoref_data.robots_yellow[new_robot_id.id()].vel_y = tracked_frame.robots(i).vel().y()*1000; // mm/s
                            my_autoref_data.robots_yellow[new_robot_id.id()].vel_angular = tracked_frame.robots(i).vel_angular(); //rad/s
                            my_vision_data.robots_yellow[new_robot_id.id()].orientation = tracked_frame.robots(i).orientation();
                        }
                        else{
                            std::cout << "!!!! há um robô com ID de time desconhecido !!!!" << std::endl;
                        }
                    }
                }
            
                if (tracked_frame.balls_size() > 0) {
                    for(int i = 0; i < tracked_frame.balls_size(); i++) {
                        if(tracked_frame.balls(i).has_vel()) {
                            Vector3 ball_vel = tracked_frame.balls(i).vel();
                            my_autoref_data.balls.vel_x = ball_vel.x()*1000;//multiplicando por 1000 para converter de metros para milimetros
                            my_autoref_data.balls.vel_y = ball_vel.y()*1000;
                        }
                        if(autoreferee_atual) {
                            Vector3 ball_pos = tracked_frame.balls(i).pos();
                            my_vision_data.balls.position_x = ball_pos.x()*1000;
                            my_vision_data.balls.position_y = ball_pos.y()*1000;
                        }
                    }
                }
            }
        }
    }
    //std::cout << "Timestamp: " << my_autoref_data.timestamp << std::endl;
}


