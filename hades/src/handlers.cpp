//#include <lcm/lcm-cpp.hpp>
//#include <iostream>
//#include <csignal>
//#include <vector>
//#include <memory>
//#include <cstdlib>
//#include "include/handlers.hpp"
//
//class Handler {
//    public:
//        void handle_GC(const lcm::ReceiveBuffer* rbuf,
//                       const std::string& chan,
//                       const data::game_controller* msg_GC) {
//            system("clear");
//            cout << "\n----------------------------------------\n";
//            cout << "            C E R B E R U S \n";
//            cout << "----------------------------------------" << endl;
//    
//            cout << "Received message on channel \"" << chan << "\"" << endl;
//            g.team_blue = msg_GC->team_blue;
//            cout << "   time_azul = " << g.team_blue << "\n" << endl; // 0 para false, 1 para true
//            cout << "score team blue: " << msg_GC->blue.score << "\n" << endl;
//        }
//    
//        void handle_vision(const lcm::ReceiveBuffer* rbuf,
//                           const std::string& chan,
//                           const data::vision* msg_vision) {
//            cout << "Received message on channel \"" << chan << "\"" << endl;
//    
//            cout << "   robots_yellow_size = " << msg_vision->robots_yellow_size << endl;
//            cout << "   robots_blue_size = " << msg_vision->robots_blue_size << "\n" << endl;
//            cout << "   " << msg_vision->timestamp << " \n" << endl;
//    
//            msg_ia.timestamp = msg_vision->timestamp;
//    
//            const std::vector<data::detection_robots>& robots_source = g.team_blue
//                ? msg_vision->robots_blue
//                : msg_vision->robots_yellow;
//    
//            msg_ia.robots_size = g.team_blue
//                ? msg_vision->robots_blue_size
//                : msg_vision->robots_yellow_size;
//    
//            msg_ia.robots.clear();
//    
//            for (int i = 0; i < msg_ia.robots_size; ++i) {
//                data::robot robots_ia;
//                const data::detection_robots& robots_sour = robots_source[i];
//    
//                robots_ia.id = robots_sour.robot_id;
//                robots_ia.spinner = false;
//                robots_ia.kick = false;
//    
//                robots_ia.vel_tang = 0;
//                robots_ia.vel_normal = 0;
//                robots_ia.vel_ang = 0;
//    
//                robots_ia.kick_speed_x = 0;
//                robots_ia.kick_speed_z = 0;
//    
//                robots_ia.wheel_speed = true;
//                robots_ia.wheel_fr = -50;
//                robots_ia.wheel_fl = 0;
//                robots_ia.wheel_bl = -50;
//                robots_ia.wheel_br = 0;
//    
//                msg_ia.robots.push_back(robots_ia);
//            }
//    
//            cout << "Field length: " << msg_vision->field.field_length << endl;
//            lc->publish("IA", &msg_ia);
//            cout << "Bola:  pos_x" << msg_vision->balls.position_x
//                 << "  pos_y" << msg_vision->balls.position_y << "\n" << endl;
//    
//            cout << "Sent message on channel IA" << endl;
//        }
//    
//        void handle_tartarus(const lcm::ReceiveBuffer* rbuf,
//                             const std::string& chan,
//                             const data::tartarus* msg_tartarus) {
//            cout << "Received message on channel \"" << chan << "\"" << endl;
//            data::tartarus new_tartarus;
//            new_tartarus.estrategia = msg_tartarus->estrategia;
//            new_tartarus.processo = msg_tartarus->processo;
//            new_tartarus.ssl_vision = msg_tartarus->ssl_vision;
//            new_tartarus.processo = 1;
//            new_tartarus.estrategia = 1;
//            cout << "   estrategia -> " << msg_tartarus->estrategia << "\n" << endl;
//            cout << "   processo -> " << msg_tartarus->processo << "\n" << endl;
//            cout << "   ssl_vision -> " << (int)msg_tartarus->ssl_vision << "\n" << endl; // 0 para false, 1 para true
//            std::this_thread::sleep_for(std::chrono::milliseconds(30));
//            lc->publish("tartarus", &new_tartarus);
//            cout << "Sent message on channel tartarus" << endl;
//        }
//    
//        lcm::LCM* lc;
//    };