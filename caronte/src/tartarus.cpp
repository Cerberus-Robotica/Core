//#include "include/tartarus.hpp"
//#include <lcm/lcm-cpp.hpp>
//#include <iostream>
//
//data::tartarus my_tartarus_data;
//
//using namespace std;
//
//void handle_tartarus(const lcm::ReceiveBuffer* rbuf,
//                    const std::string& chan,
//                    const data::tartarus* msg_tartarus) {
//    cout << "\n----------------------------------------\n";
//    cout << "            C E R B E R U S \n";
//    cout << "----------------------------------------" << endl;
//    cout << "Received message on channel \"" << chan << "\"" << endl;
//    cout << "   estrategia -> " << msg_tartarus->estrategia << "\n" << endl; // 0 para false, 1 para true
//    cout << "   processo -> " << msg_tartarus->processo << "\n" << endl;
//    cout << "   ssl_vision -> " << msg_tartarus->ssl_vision << "\n" << endl; // 0 para false, 1 para true
//}