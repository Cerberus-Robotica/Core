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


// ======= Dados Compartilhados =======
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

struct DetectionRobots {
    int16_t robot_id;
    float position_x;
    float position_y;
    float orientation;
};

struct DetectionBalls {
    float position_x;
    float position_y;
};

struct DetectionGeometry {
    int32_t field_length;
    int32_t field_width;
    int32_t goal_width;
    int32_t goal_depth;
    int32_t boundary_width;
    int32_t center_circle_radius;
    int32_t defense_area_width;
    int32_t defense_area_height;
    int32_t line_thickness;
    int32_t goal_center_to_penalty_mark;
    int32_t goal_height;
    float ball_radius;
    float max_robot_radius;
};

struct LatestData {
    // GC (Game Controller)
    bool team_blue = false;
    float designated_position_x = 0;
    float designated_position_y = 0;
    int current_command = 0;
    int game_event = 0;

    TeamInfo yellow;
    TeamInfo blue;

    // IA
    int robots_size = 0;
    std::string processo;
    std::string estrategia;
    std::vector<RobotData> robots;

    // Vision
    int64_t timestamp = 0;
    int16_t robots_yellow_size;
    int16_t robots_blue_size;
    std::vector<DetectionRobots> robots_yellow;
    std::vector<DetectionRobots> robots_blue;
    DetectionBalls balls;
    DetectionGeometry field;

    // Tartarus
    bool ssl_vision = false;
    bool competition_mode = false;
    bool bool_controller = false;
    int16_t stm_port;
    bool team_blue_status = false;
} latest_data;


// ======= Handlers LCM =======
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

    // Game Controller
    void handleGC(const lcm::ReceiveBuffer*, const std::string&, const gc_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        // Atualização dos dados GC...
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

    // Vision
    void handleVision(const lcm::ReceiveBuffer*, const std::string&, const vision_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        // Atualização dos dados Vision...
        msg_vision = *msg;
        latest_data.timestamp = msg_vision.timestamp;

        latest_data.robots_yellow.clear();
        latest_data.robots_yellow_size = msg_vision.robots_yellow_size;
        for (int i = 0; i < msg_vision.robots_yellow_size; ++i) {
            DetectionRobots dr;
            dr.robot_id = msg_vision.robots_yellow[i].robot_id;
            dr.position_x = msg_vision.robots_yellow[i].position_x;
            dr.position_y = msg_vision.robots_yellow[i].position_y;
            dr.orientation = msg_vision.robots_yellow[i].orientation;
            latest_data.robots_yellow.push_back(dr);
        }

        latest_data.robots_blue.clear();
        latest_data.robots_blue_size = msg_vision.robots_blue_size;
        for (int i = 0; i < msg_vision.robots_blue_size; ++i) {
            DetectionRobots db;
            db.robot_id = msg_vision.robots_blue[i].robot_id;
            db.position_x = msg_vision.robots_blue[i].position_x;
            db.position_y = msg_vision.robots_blue[i].position_y;
            db.orientation = msg_vision.robots_blue[i].orientation;
            latest_data.robots_blue.push_back(db);
        }

        latest_data.balls.position_x = msg_vision.balls.position_x;
        latest_data.balls.position_y = msg_vision.balls.position_y;

        // Campo
        latest_data.field.field_length = msg_vision.field.field_length;
        latest_data.field.field_width = msg_vision.field.field_width;
        latest_data.field.goal_width = msg_vision.field.goal_width;
        latest_data.field.goal_depth = msg_vision.field.goal_depth;
        latest_data.field.boundary_width = msg_vision.field.boundary_width;
        latest_data.field.center_circle_radius = msg_vision.field.center_circle_radius;
        latest_data.field.defense_area_width = msg_vision.field.defense_area_width;
        latest_data.field.defense_area_height = msg_vision.field.defense_area_height;
        latest_data.field.line_thickness = msg_vision.field.line_thickness;
        latest_data.field.goal_center_to_penalty_mark = msg_vision.field.goal_center_to_penalty_mark;
        latest_data.field.goal_height = msg_vision.field.goal_height;
        latest_data.field.ball_radius = msg_vision.field.ball_radius;
        latest_data.field.max_robot_radius = msg_vision.field.max_robot_radius;
    }

    // Tartarus
    void handleTartarus(const lcm::ReceiveBuffer*, const std::string&, const tartarus_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_tartarus = *msg;

        if (msg_GC.blue.name == "Cerberus") {
            latest_data.team_blue_status = true;
            latest_data.blue.goalkeeper_id = msg_tartarus.goalkeeper_id;
        } else if (msg_GC.yellow.name == "Cerberus") {
            latest_data.team_blue_status = false;
            latest_data.yellow.goalkeeper_id = msg_tartarus.goalkeeper_id;
        } else {
            latest_data.team_blue_status = true;
            latest_data.blue.goalkeeper_id = msg_tartarus.goalkeeper_id;
        }

        latest_data.ssl_vision = msg_tartarus.ssl_vision;
        latest_data.competition_mode = msg_tartarus.competition_mode;
        latest_data.bool_controller = msg_tartarus.bool_controller;
        latest_data.stm_port = msg_tartarus.stm_port;
    }

    // IA
    void handleIA(const lcm::ReceiveBuffer*, const std::string&, const ia_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_ia = *msg;

        latest_data.robots_size = msg->robots_size;
        latest_data.processo = msg->processo;
        latest_data.estrategia = msg->estrategia;
        latest_data.timestamp = msg->timestamp;

        int safe_size = std::max(0, std::min(static_cast<int>(msg->robots_size), 16));

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


// ======= Thread de leitura LCM =======
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


// ======= Middleware CORS =======
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


// ======= Rotas HTTP =======
int main() {
    std::thread lcm_thread(lcm_thread_func);
    crow::App<CORS> app;

    CROW_ROUTE(app, "/data").methods("GET"_method)([] {
        std::lock_guard<std::mutex> lock(data_mutex);

        crow::json::wvalue data;

        // GC
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

        // IA
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

        // Vision - Yellow
        for (size_t i = 0; i < latest_data.robots_yellow.size(); ++i) {
            const auto& r = latest_data.robots_yellow[i];
            crow::json::wvalue robot;
            robot["robot_id"] = r.robot_id;
            robot["position_x"] = r.position_x;
            robot["position_y"] = r.position_y;
            robot["orientation"] = r.orientation;
            data["robots_yellow"][i] = std::move(robot);
        }

        // Vision - Blue
        for (size_t i = 0; i < latest_data.robots_blue.size(); ++i) {
            const auto& r = latest_data.robots_blue[i];
            crow::json::wvalue robot;
            robot["robot_id"] = r.robot_id;
            robot["position_x"] = r.position_x;
            robot["position_y"] = r.position_y;
            robot["orientation"] = r.orientation;
            data["robots_blue"][i] = std::move(robot);
        }

        // Ball
        data["balls"]["position_x"] = latest_data.balls.position_x;
        data["balls"]["position_y"] = latest_data.balls.position_y;

        // Field
        const auto& f = latest_data.field;
        data["field"]["field_length"] = f.field_length;
        data["field"]["field_width"] = f.field_width;
        data["field"]["goal_width"] = f.goal_width;
        data["field"]["goal_depth"] = f.goal_depth;
        data["field"]["boundary_width"] = f.boundary_width;
        data["field"]["center_circle_radius"] = f.center_circle_radius;
        data["field"]["defense_area_width"] = f.defense_area_width;
        data["field"]["defense_area_height"] = f.defense_area_height;
        data["field"]["line_thickness"] = f.line_thickness;
        data["field"]["goal_center_to_penalty_mark"] = f.goal_center_to_penalty_mark;
        data["field"]["goal_height"] = f.goal_height;
        data["field"]["ball_radius"] = f.ball_radius;
        data["field"]["max_robot_radius"] = f.max_robot_radius;

        // Extras
        data["processo"] = latest_data.processo;
        data["estrategia"] = latest_data.estrategia;
        data["timestamp"] = latest_data.timestamp;
        data["robots_size"] = latest_data.robots_size;
        data["ssl_vision"] = latest_data.ssl_vision;
        data["team_blue_status"] = latest_data.team_blue_status;
        data["competition_mode"] = latest_data.competition_mode;
        data["bool_controller"] = latest_data.bool_controller;

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

            if (body.has("team_blue") && (body["team_blue"].t() == crow::json::type::True || body["team_blue"].t() == crow::json::type::False)) {
                latest_data.team_blue = body["team_blue"].b();
                std::cout << "[POST] team_blue atualizado para " << (latest_data.team_blue ? "azul" : "amarelo") << std::endl;
            }

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

            // Extras
            if (body.has("ssl_vision") && (body["ssl_vision"].t() == crow::json::type::True || body["ssl_vision"].t() == crow::json::type::False)) {
                latest_data.ssl_vision = body["ssl_vision"].b();
                std::cout << "[POST] ssl_vision atualizado para " << (latest_data.ssl_vision ? "true" : "false") << std::endl;
            }

            if (body.has("competition_mode") && (body["competition_mode"].t() == crow::json::type::True || body["competition_mode"].t() == crow::json::type::False)) {
                latest_data.competition_mode = body["competition_mode"].b();
                std::cout << "[POST] competition_mode atualizado para " << (latest_data.competition_mode ? "true" : "false") << std::endl;
            }

            if (body.has("bool_controller") && (body["bool_controller"].t() == crow::json::type::True || body["bool_controller"].t() == crow::json::type::False)) {
                latest_data.bool_controller = body["bool_controller"].b();
                std::cout << "[POST] bool_controller atualizado para " << (latest_data.bool_controller ? "true" : "false") << std::endl;
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
