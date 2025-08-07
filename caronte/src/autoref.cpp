#include "include/autoref.hpp"
#include "include/socket_connect.hpp"
#include "include/vision.hpp"
#include "include/tartarus.hpp"
#include <iostream>
#include <unordered_map>
#include <thread>

data::autoref my_autoref_data;

void recebe_dados_tracked() {
    lcm::LCM lcm;

    TrackerWrapperPacket tracked_packet;
    TrackedFrame tracked_frame;

    data::detection_robots_tracked new_robot;

    std::unordered_set<int> yellow_ids;
    std::unordered_set<int> blue_ids; // Conjunto para armazenar IDs únicos, de robôs
    bool ssl_vision_atual;
    int cameras;
    
    std::cout << "Connecting to tigers autoreferee..." << std::endl;
    while(true){
        struct sockaddr_in sender_addr;
        socklen_t addr_len = sizeof(sender_addr);       
        
        if(ssl_vision_atual != han.new_tartarus.ssl_vision) {
            //close(sock_TRACKED);
            //setupTrackedSocket();
            ssl_vision_atual = han.new_tartarus.ssl_vision;
        }

        //std::cout << "ssl_vision_atual: " << ssl_vision_atual << std::endl;
        //std::cout << "han.new_tartarus.ssl_vision: " << (int)han.new_tartarus.ssl_vision << std::endl;
        cameras = han.new_tartarus.ssl_vision ? 1 : 4; // 4 cameras para o grsim e 1 camera para o ssl-vision

        for(int i = 0; i < cameras ; i++){ // usar 4 para grsim e usar 1 para ssl-vision
            
            int bytes_received_TRACKED = recvfrom(sock_TRACKED, buffer_TRACKED, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
            //std::cout << "bytes_received_TRACKED: " << bytes_received_TRACKED << std::endl;

            if (bytes_received_TRACKED > 0) {
                // Parse dos dados recebidos (Tracked)
                tracked_packet.ParseFromArray(buffer_TRACKED, bytes_received_TRACKED);

                
                if(tracked_packet.has_tracked_frame()){
                    my_autoref_data.timestamp = tracked_packet.tracked_frame().timestamp();
                    tracked_frame = tracked_packet.tracked_frame();
                    //std::cout << "Timestamp autoref: " << my_autoref_data.timestamp << std::endl;
                    //std::cout << "tracked_frame.robots_size(): " << tracked_frame.robots_size() << std::endl;
                    //std::cout << "tracked_frame.balls_size(): " << tracked_frame.balls_size() << std::endl;

                    if (tracked_frame.robots_size() > 0) {
                        for (int i = 0; i < tracked_frame.robots_size(); i++) {
                            RobotId new_robot_id = tracked_frame.robots(i).robot_id();
                            if(new_robot_id.team() == 1){ //UNKNOWN = 0, YELLOW = 1, BLUE = 2
                            
                                int id = new_robot_id.id();
                    
                                if (yellow_ids.find(id) == yellow_ids.end()) {
                                    
                                    new_robot.robot_id = id;
                                    new_robot.position_x = tracked_frame.robots(i).pos().x()*1000; //multiplicando por 1000 para converter de metros para milimetros
                                    new_robot.position_y = tracked_frame.robots(i).pos().y()*1000;
                                    new_robot.vel_x = tracked_frame.robots(i).vel().x()*1000;
                                    new_robot.vel_y = tracked_frame.robots(i).vel().y()*1000;
                                    new_robot.vel_angular = tracked_frame.robots(i).vel_angular()*1000;
                                    new_robot.orientation = tracked_frame.robots(i).orientation();
                                    my_autoref_data.robots_yellow.push_back(new_robot);
                                    yellow_ids.insert(id);
                                }
                            }
                            
                            else if(new_robot_id.team() == 2){ //UNKNOWN = 0, YELLOW = 1, BLUE = 2
                            
                                int id = new_robot_id.id();
                    
                                if (blue_ids.find(id) == blue_ids.end()) {
                                    
                                    new_robot.robot_id = id;
                                    new_robot.position_x = tracked_frame.robots(i).pos().x()*1000; //multiplicando por 1000 para converter de metros para milimetros
                                    new_robot.position_y = tracked_frame.robots(i).pos().y()*1000;
                                    new_robot.vel_x = tracked_frame.robots(i).vel().x()*1000;
                                    new_robot.vel_y = tracked_frame.robots(i).vel().y()*1000;
                                    new_robot.vel_angular = tracked_frame.robots(i).vel_angular()*1000;
                                    new_robot.orientation = tracked_frame.robots(i).orientation();
                                    my_autoref_data.robots_blue.push_back(new_robot);
                                    blue_ids.insert(id);
                                }
                            }
                            else{
                                std::cout << "!!!! há um robô com ID desconhecido !!!!" << std::endl;
                            }
                        }
                    }
                    if (tracked_frame.balls_size() > 0) {
                        for(int i = 0; i < tracked_frame.balls_size(); i++) {
                            Vector3 ball_pos = tracked_frame.balls(i).pos();
                            my_autoref_data.balls.position_x = ball_pos.x()*1000; //multiplicando por 1000 para converter de metros para milimetros
                            my_autoref_data.balls.position_y = ball_pos.y()*1000;
                            //std::cout << "Ball position " << i << ": " << ball_pos.x()*1000 << ", " << my_autoref_data.balls.position_y*1000 << std::endl;
                        }
                    }
                }
            }
        }
        my_autoref_data.robots_yellow_size = yellow_ids.size();
        my_autoref_data.robots_blue_size = blue_ids.size();

        //for(int i = 0; i < my_autoref_data.robots_blue_size; i++){
        //    std::cout << "Robô azul ID: " << my_autoref_data.robots_blue[i].robot_id << std::endl;
        //    std::cout << "Posição X: " << my_autoref_data.robots_blue[i].position_x << std::endl;
        //    std::cout << "Posição Y: " << my_autoref_data.robots_blue[i].position_y << std::endl;
        //    std::cout << "Orientação: " << my_autoref_data.robots_blue[i].orientation << "\n" << std::endl;
        //}
    //
        //for(int i = 0; i < my_autoref_data.robots_yellow_size; i++){
        //    std::cout << "Robô amarelo ID: " << my_autoref_data.robots_yellow[i].robot_id << std::endl;
        //    std::cout << "Posição X: " << my_autoref_data.robots_yellow[i].position_x << std::endl;
        //    std::cout << "Posição Y: " << my_autoref_data.robots_yellow[i].position_y << std::endl;
        //    std::cout << "Orientação: " << my_autoref_data.robots_yellow[i].orientation << "\n" << std::endl;
        //}
//
        //std::cout << "Robos azuis: " << my_autoref_data.robots_blue_size;
        //std::cout << "  Robos amarelos: " << my_autoref_data.robots_yellow_size << std::endl;
        //std::cout << "Timestamp: " << my_autoref_data.timestamp << std::endl;
    

        lcm.publish("vision_tracked", &my_autoref_data);

        yellow_ids.clear();
        blue_ids.clear();
        my_autoref_data.robots_yellow.clear();
        my_autoref_data.robots_blue.clear();
    }
}
