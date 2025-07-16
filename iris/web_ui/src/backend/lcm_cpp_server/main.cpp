#define CROW_MAIN
#include "crow_all.h"

#include <lcm/lcm-cpp.hpp>
#include <mutex>
#include <thread>
#include <iostream>

#include "../../../../../data_lcm/data/game_controller.hpp"
#include "../../../../../data_lcm/data/vision.hpp"
#include "../../../../../data_lcm/data/tartarus.hpp"
#include "../../../../../data_lcm/data/ia.hpp"
#include "../../../../../data_lcm/data/team_info.hpp"

std::mutex data_mutex;

struct TeamInfo {
    std::string name;
    int score = 0;
    int fouls = 0;
    int goalkeeper_id = 0;
};

struct RobotData {
    int16_t id;
    bool spinner;
    bool kick;
    float vel_tang;
    float vel_normal;
    float vel_ang;
    float kick_speed_x;
    float kick_speed_z;
    bool wheel_speed;
    float wheel_fr;
    float wheel_fl;
    float wheel_bl;
    float wheel_br;
};

struct LatestData {
    //AutoRef:

    //GC:
    bool team_blue = false;
    float designated_position_x = 0;
    float designated_position_y = 0;
    int current_command = 0;
	int game_event = 0;

    TeamInfo yellow;
    TeamInfo blue;

    //IA:
    int robots_size = 0;
    std::vector<RobotData> robots;
    

    int64_t timestamp = 0;
    
    int score_yellow = 0;
    int score_blue = 0;
    float field_length = 0.0f;
    std::string processo;
    std::string estrategia;
    bool ssl_vision = false;
    bool team_blue_status = false;
    int goalkeeper_id = -1;  // novo campo
} latest_data;

class Handler {
public:
    using gc_t = data::game_controller;
    using vision_t = data::vision;
    using tartarus_t = data::tartarus;
    using ia_t = data::ia;

    gc_t msg_GC;
    vision_t msg_vision;
    tartarus_t msg_tartarus;
    ia_t msg_ia;

    void handleGC(const lcm::ReceiveBuffer*, const std::string&, const gc_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_GC = *msg;
        latest_data.team_blue = msg_GC.team_blue;
        latest_data.designated_position_x = msg_GC.designated_position_x;
        latest_data.designated_position_y = msg_GC.designated_position_y;
        latest_data.current_command = msg_GC.current_command;
        latest_data.game_event = msg_GC.game_event;

        latest_data.blue.name = msg_GC.blue.name;
        latest_data.blue.score = msg_GC.blue.score;
        latest_data.blue.fouls = msg_GC.blue.fouls;
        latest_data.blue.goalkeeper_id = msg_GC.blue.goalkeeper_id;

        latest_data.yellow.name = msg_GC.yellow.name;
        latest_data.yellow.score = msg_GC.yellow.score;
        latest_data.yellow.fouls = msg_GC.yellow.fouls;
        latest_data.yellow.goalkeeper_id = msg_GC.yellow.goalkeeper_id;
    }

    void handleVision(const lcm::ReceiveBuffer*, const std::string&, const vision_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_vision = *msg;
        latest_data.timestamp = msg_vision.timestamp;
        latest_data.field_length = msg_vision.field.field_length;
    }

    void handleTartarus(const lcm::ReceiveBuffer*, const std::string&, const tartarus_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_tartarus = *msg;
        if (msg_GC.blue.name == "Cerberus") latest_data.team_blue_status = true;
        else if (msg_GC.yellow.name == "Cerberus") latest_data.team_blue_status = false;
        else latest_data.team_blue_status = true;
        latest_data.ssl_vision = msg_tartarus.ssl_vision;
        latest_data.goalkeeper_id=msg_tartarus.goalkeeper_id;
    }

    void handleIA(const lcm::ReceiveBuffer*, const std::string&, const ia_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_ia = *msg;

        latest_data.robots_size = msg->robots_size;
        latest_data.processo = msg->processo;
        latest_data.estrategia = msg->estrategia;
        latest_data.timestamp = msg->timestamp;

        // Garante que não haja valores inválidos
        int safe_size = std::max(0, std::min(static_cast<int>(msg->robots_size), 16));// limite arbitrário de segurança

        latest_data.robots.clear();

        for (int i = 0; i < safe_size; ++i) {
            const auto& robot = msg->robots[i];
            RobotData r;

            r.id = robot.id;
            r.spinner = robot.spinner;
            r.kick = robot.kick;
            r.vel_tang = robot.vel_tang;
            r.vel_normal = robot.vel_normal;
            r.vel_ang = robot.vel_ang;
            r.kick_speed_x = robot.kick_speed_x;
            r.kick_speed_z = robot.kick_speed_z;
            r.wheel_speed = robot.wheel_speed;
            r.wheel_fr = robot.wheel_fr;
            r.wheel_fl = robot.wheel_fl;
            r.wheel_bl = robot.wheel_bl;
            r.wheel_br = robot.wheel_br;

            latest_data.robots.push_back(r);
        }
    }
};

void lcm_thread_func() {
    lcm::LCM lc;
    if (!lc.good()) {
        std::cerr << "Erro ao inicializar LCM" << std::endl;
        return;
    }

    Handler handler;
    lc.subscribe("GC", &Handler::handleGC, &handler);
    lc.subscribe("vision", &Handler::handleVision, &handler);
    lc.subscribe("tartarus", &Handler::handleTartarus, &handler);
    lc.subscribe("IA", &Handler::handleIA, &handler);

    while (lc.handle() == 0) {}
}

struct CORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        if (req.method == "OPTIONS"_method) {
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};

int main() {
    std::thread lcm_thread(lcm_thread_func);
    crow::App<CORS> app;

    CROW_ROUTE(app, "/data").methods("GET"_method)([] {
        std::lock_guard<std::mutex> lock(data_mutex);

        crow::json::wvalue data;

        data["team_blue"] = latest_data.team_blue;
        data["designated_position_x"] = latest_data.designated_position_x;
        data["designated_position_y"] = latest_data.designated_position_y;
        data["current_command"] = latest_data.current_command;
        data["game_event"] = latest_data.game_event;

        data["blue"]["name"] = latest_data.blue.name;
        data["blue"]["score"] = latest_data.blue.score;
        data["blue"]["fouls"] = latest_data.blue.fouls;
        data["blue"]["goalkeeper_id"] = latest_data.blue.goalkeeper_id;

        data["yellow"]["name"] = latest_data.yellow.name;
        data["yellow"]["score"] = latest_data.yellow.score;
        data["yellow"]["fouls"] = latest_data.yellow.fouls;
        data["yellow"]["goalkeeper_id"] = latest_data.yellow.goalkeeper_id;

        for (size_t i = 0; i < latest_data.robots.size(); ++i) {
            const auto& r = latest_data.robots[i];
            crow::json::wvalue robot;
            robot["id"] = r.id;
            robot["spinner"] = r.spinner;
            robot["kick"] = r.kick;
            robot["vel_tang"] = r.vel_tang;
            robot["vel_normal"] = r.vel_normal;
            robot["vel_ang"] = r.vel_ang;
            robot["kick_speed_x"] = r.kick_speed_x;
            robot["kick_speed_z"] = r.kick_speed_z;
            robot["wheel_speed"] = r.wheel_speed;
            robot["wheel_fr"] = r.wheel_fr;
            robot["wheel_fl"] = r.wheel_fl;
            robot["wheel_bl"] = r.wheel_bl;
            robot["wheel_br"] = r.wheel_br;
            data["robots"][i] = std::move(robot);
        }


        data["processo"] = latest_data.processo;
        data["estrategia"] = latest_data.estrategia;
        data["timestamp"] = latest_data.timestamp;
        data["field_length"] = latest_data.field_length;
        data["robots_size"] = latest_data.robots_size;
        data["ssl_vision"] = latest_data.ssl_vision;
        data["team_blue_status"] = latest_data.team_blue_status;

        return crow::response{data};
    });


    CROW_ROUTE(app, "/command").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            std::cerr << "JSON inválido: " << req.body << std::endl;
            return crow::response(400, "JSON inválido");
        }

        std::lock_guard<std::mutex> lock(data_mutex);

        try {
            std::cout << "JSON recebido: " << req.body << std::endl;

            // Atualiza team_blue se enviado
            if (body.has("team_blue") &&
                (body["team_blue"].t() == crow::json::type::True || body["team_blue"].t() == crow::json::type::False)) {
                latest_data.team_blue = body["team_blue"].b();
                std::cout << "[POST] team_blue atualizado para " << (latest_data.team_blue ? "azul" : "amarelo") << std::endl;
            }


            // Atualiza goalkeeper_id no time selecionado
            if (body.has("goalkeeper_id") && body["goalkeeper_id"].t() == crow::json::type::Number) {
                int id = body["goalkeeper_id"].i();
                if (latest_data.team_blue) {
                    latest_data.blue.goalkeeper_id = id;
                    std::cout << "[POST] goalkeeper_id AZUL atualizado para " << id << std::endl;
                } else {
                    latest_data.yellow.goalkeeper_id = id;
                    std::cout << "[POST] goalkeeper_id AMARELO atualizado para " << id << std::endl;
                }
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar POST: " << e.what() << std::endl;
            return crow::response(500, "Erro interno");
        }

        return crow::response(200, "Comando recebido com sucesso");
    });






    app.port(5000).multithreaded().run();
    lcm_thread.join();
    return 0;
}
