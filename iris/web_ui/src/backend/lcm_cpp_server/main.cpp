#define CROW_MAIN
#include "crow_all.h"

#include <lcm/lcm-cpp.hpp>
#include <mutex>
#include <thread>
#include <iostream>
#include "handlers/handlers.hpp"

lcm::LCM global_lcm; // LCM para publicar comandos

// ======= Dados Compartilhados =======
std::mutex data_mutex;
LatestData latest_data;

// ======= Thread de leitura LCM =======
void lcm_thread_func()
{
    lcm::LCM lc;
    if (!lc.good())
    {
        std::cerr << "Erro ao inicializar LCM" << std::endl;
        return;
    }

    Handler handler;
    lc.subscribe("GC", &Handler::handleGC, &handler);
    lc.subscribe("vision", &Handler::handleVision, &handler);
    lc.subscribe("tartarus", &Handler::handleTartarus, &handler);
    lc.subscribe("IA", &Handler::handleIA, &handler);

    while (lc.handle() == 0)
    {
    }
}

// ======= Middleware CORS =======
struct CORS
{
    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &)
    {
        if (req.method == "OPTIONS"_method)
        {
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request &, crow::response &res, context &)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};

// ======= Rotas HTTP =======
int main()
{
    if (!global_lcm.good())
    {
        std::cerr << "Erro ao inicializar o LCM publisher" << std::endl;
        return 1;
    }

    std::thread lcm_thread(lcm_thread_func);
    crow::App<CORS> app;

    CROW_ROUTE(app, "/data").methods("GET"_method)([]
                                                   {
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
        data["stm_port"] = latest_data.stm_port;
        data["controller_port"] = latest_data.controller_port;

        return crow::response{data}; });

    CROW_ROUTE(app, "/command").methods("POST"_method)([](const crow::request &req)
                                                       {
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
            if (body.has("stm_port") && body["stm_port"].t() == crow::json::type::Number) {
                latest_data.stm_port = body["stm_port"].i();
                std::cout << "[POST] stm_port atualizado para " << latest_data.stm_port << std::endl;
            }

            if (body.has("controller_port") && body["controller_port"].t() == crow::json::type::Number) {
                latest_data.controller_port = body["controller_port"].i();
                std::cout << "[POST] controller_port atualizado para " << latest_data.controller_port << std::endl;
            }

            // ===== PUBLICAÇÃO TARTARUS =====
            data::tartarus msg;
            msg.ssl_vision = latest_data.ssl_vision;
            msg.competition_mode = latest_data.competition_mode;
            msg.bool_controller = latest_data.bool_controller;
            msg.goalkeeper_id = latest_data.team_blue ? latest_data.blue.goalkeeper_id : latest_data.yellow.goalkeeper_id;
            msg.stm_port = latest_data.stm_port;
            msg.controller_port = latest_data.controller_port;

            global_lcm.publish("tartarus", &msg);
            std::cout << "[POST] Mensagem publicada no canal 'tartarus'\n";


        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar POST: " << e.what() << std::endl;
            return crow::response(500, "Erro interno");
        }

        return crow::response(200, "Comando recebido com sucesso"); });

    app.port(5000).multithreaded().run();
    lcm_thread.join();
    return 0;
}