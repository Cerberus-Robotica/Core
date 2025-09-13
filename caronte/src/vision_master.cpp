#include "include/vision_master.hpp"
#include "include/tartarus.hpp"

vision_master vision_master_instance;

void vision_master::tartarus_updated() {
    if(han.updated_tartarus != vision_master_instance.updated) {
        vision_master_instance.updated = !vision_master_instance.updated;
    }
}

void vision_master::vision_main()
{
    while (true)
    {
        tartarus_updated();
        //when ssl_vision_atual changes, close the old socket and setup a new one to connect to other software
        if(vision_master_instance.ssl_vision_atual != han.new_tartarus.ssl_vision) {
            close(sock_vision);
            setupVisionSocket();
            vision_master_instance.ssl_vision_atual = han.new_tartarus.ssl_vision;
        }
        if(autoreferee_atual != han.new_tartarus.autoreferee) {
            close(sock_TRACKED);
            setupTrackedSocket();
            autoreferee_atual = han.new_tartarus.autoreferee;
        }

        if(han.new_tartarus.cams_number > 0){
            vision_master_instance.cameras = han.new_tartarus.cams_number;
        }
        else{
            vision_master_instance.cameras = han.new_tartarus.ssl_vision ? 1 : 4; // 4 cameras para o grsim e 1 camera para o ssl-vision
        }
//        std::cout << "Cameras: " << vision_master_instance.cameras << std::endl;

        vision_master_instance.recebe_dados_vision();
        if(autoreferee_atual){
            vision_master_instance.recebe_dados_tracked();
        }

//        for(int i = 0; i < 16; i++){
//            if(my_vision_data.robots_blue[i].detected == true) {
//                std::cout << "Robô azul ID: " << my_vision_data.robots_blue[i].robot_id << std::endl;
//                std::cout << "Posição X: " << my_vision_data.robots_blue[i].position_x << std::endl;
//                std::cout << "Posição Y: " << my_vision_data.robots_blue[i].position_y << std::endl;
//                std::cout << "Orientação: " << my_vision_data.robots_blue[i].orientation << "\n" << std::endl;
//            }
//        }
//        for(int i = 0; i < 16; i++){
//            if(my_vision_data.robots_yellow[i].detected == true) {
//                std::cout << "Robô amarelo ID: " << my_vision_data.robots_yellow[i].robot_id << std::endl;
//                std::cout << "Posição X: " << my_vision_data.robots_yellow[i].position_x << std::endl;
//                std::cout << "Posição Y: " << my_vision_data.robots_yellow[i].position_y << std::endl;
//                std::cout << "Orientação: " << my_vision_data.robots_yellow[i].orientation << "\n" << std::endl;
//            }
//        }
//        for(int i = 0; i < 16; i++){
//            if(my_autoref_data.robots_yellow[i].vel_x != 0 || my_autoref_data.robots_yellow[i].vel_y != 0 || my_autoref_data.robots_yellow[i].vel_angular != 0) {
//                std::cout << "Robô amarelo ID: " << my_vision_data.robots_yellow[i].robot_id << std::endl;
//                std::cout << "Velocidade X: " << my_autoref_data.robots_yellow[i].vel_x << std::endl;
//                std::cout << "Velocidade Y: " << my_autoref_data.robots_yellow[i].vel_y << std::endl;
//                std::cout << "Velocidade Angular: " << my_autoref_data.robots_yellow[i].vel_angular << "\n" << std::endl;
//            }
//        }
//
//        //std::cout << "field length: " << my_vision_data.field.field_length << std::endl;    
//        std::cout << "\nball position_x " << my_vision_data.balls.position_x << std::endl;
//        std::cout << "Robos azuis: " << blue_ids.size() << std::endl;
//        std::cout << "  Robos amarelos: " << my_vision_data.robots_yellow_size << std::endl;
//        std::cout << "iris_as_gc: " << int(han.new_tartarus.iris_as_GC) << std::endl;
//        std::cout << "Timestamp: " << my_vision_data.timestamp << std::endl;
//        std::cout << "my_autoref_data.timestamp: " << my_autoref_data.timestamp << std::endl;

        lcm.publish("vision", &my_vision_data);
        lcm.publish("vision_tracked", &my_autoref_data);
        
        memset(my_vision_data.robots_blue, 0, sizeof(my_vision_data.robots_blue));
        memset(my_vision_data.robots_yellow, 0, sizeof(my_vision_data.robots_yellow));
        memset(my_autoref_data.robots_blue, 0, sizeof(my_autoref_data.robots_blue));
        memset(my_autoref_data.robots_yellow, 0, sizeof(my_autoref_data.robots_yellow));

        vision_master_instance.yellow_ids.clear();
    	vision_master_instance.blue_ids.clear();
    }
}
