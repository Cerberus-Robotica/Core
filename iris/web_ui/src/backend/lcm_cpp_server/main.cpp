#define CROW_MAIN
#include "crow_all.h"

#include <lcm/lcm-cpp.hpp>
#include <mutex>
#include <thread>
#include <iostream>
#include "handlers/handlers.hpp"

extern LCMControl lcm_control;

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

    // ============================
    // GET /data
    // ============================
    CROW_ROUTE(app, "/data").methods("GET"_method)([] {
        std::lock_guard<std::mutex> lock(data_mutex);

        crow::json::wvalue data;

        // ---- Status principais ----
        data["ssl_vision"] = latest_data.ssl_vision;
        data["autoreferee"] = latest_data.autoreferee;
        data["competition_mode"] = latest_data.competition_mode;
        data["bool_controller"] = latest_data.bool_controller;
        data["debug_mode"] = latest_data.debug_mode;
        data["half_field"] = latest_data.half_field;
        data["iris_as_GC"] = latest_data.iris_as_GC;
        data["team_blue"] = latest_data.team_blue;
        data["team_blue_status"] = latest_data.team_blue_status;

        // ---- Configuração de portas ----
        data["stm_port"] = latest_data.stm_port;
        data["mcast_port_gc"] = latest_data.mcast_port_gc;
        data["mcast_port_vision_grsim"] = latest_data.mcast_port_vision_grsim;
        data["mcast_port_vision_sslvision"] = latest_data.mcast_port_vision_sslvision;
        data["mcast_port_vision_tracked"] = latest_data.mcast_port_vision_tracked;

        // ---- Game Controller ----
        data["designated_position_x"] = latest_data.designated_position_x;
        data["designated_position_y"] = latest_data.designated_position_y;
        data["current_command"] = latest_data.current_command;
        data["game_event"] = latest_data.game_event;

        // ---- Times ----
        data["blue"]["name"] = latest_data.blue.name;
        data["blue"]["score"] = latest_data.blue.score;
        data["blue"]["fouls"] = latest_data.blue.fouls;
        data["blue"]["goalkeeper_id"] = latest_data.blue.goalkeeper_id;

        data["yellow"]["name"] = latest_data.yellow.name;
        data["yellow"]["score"] = latest_data.yellow.score;
        data["yellow"]["fouls"] = latest_data.yellow.fouls;
        data["yellow"]["goalkeeper_id"] = latest_data.yellow.goalkeeper_id;

        // ---- IA Robots ----
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

        // ---- Vision Robots Yellow ----
        for (size_t i = 0; i < latest_data.robots_yellow.size(); ++i) {
            const auto& r = latest_data.robots_yellow[i];
            crow::json::wvalue robot;
            robot["robot_id"] = r.robot_id;
            robot["position_x"] = r.position_x;
            robot["position_y"] = r.position_y;
            robot["orientation"] = r.orientation;
            data["robots_yellow"][i] = std::move(robot);
        }

        // ---- Vision Robots Blue ----
        for (size_t i = 0; i < latest_data.robots_blue.size(); ++i) {
            const auto& r = latest_data.robots_blue[i];
            crow::json::wvalue robot;
            robot["robot_id"] = r.robot_id;
            robot["position_x"] = r.position_x;
            robot["position_y"] = r.position_y;
            robot["orientation"] = r.orientation;
            data["robots_blue"][i] = std::move(robot);
        }

        // ---- Bola ----
        data["balls"]["position_x"] = latest_data.balls.position_x;
        data["balls"]["position_y"] = latest_data.balls.position_y;

        // ---- Campo ----
        const auto& f = latest_data.field;
        data["field"]["field_length"] = f.field_length;
        data["field"]["field_width"] = f.field_width;
        data["field"]["goal_width"] = f.goal_width;
        data["field"]["goal_depth"] = f.goal_depth;
        data["field"]["goal_height"] = f.goal_height;
        data["field"]["boundary_width"] = f.boundary_width;
        data["field"]["center_circle_radius"] = f.center_circle_radius;
        data["field"]["defense_area_width"] = f.defense_area_width;
        data["field"]["defense_area_height"] = f.defense_area_height;
        data["field"]["line_thickness"] = f.line_thickness;
        data["field"]["goal_center_to_penalty_mark"] = f.goal_center_to_penalty_mark;
        data["field"]["ball_radius"] = f.ball_radius;
        data["field"]["max_robot_radius"] = f.max_robot_radius;

        // ---- Extras ----
        data["processo"] = latest_data.processo;
        data["estrategia"] = latest_data.estrategia;
        data["timestamp"] = latest_data.timestamp;
        data["robots_size"] = latest_data.robots_size;
        data["cams_number"] = latest_data.cams_number;

        return crow::response{data};
    });

    // ============================
    // POST /command
    // ============================
    CROW_ROUTE(app, "/command").methods("POST"_method)([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            std::cerr << "JSON inválido: " << req.body << std::endl;
            return crow::response(400, "JSON inválido");
        }

        std::lock_guard<std::mutex> lock(data_mutex);

        try
        {
            // Atualiza team_blue manualmente e desativa fonte LCM
            if (body.has("team_blue"))
            {
                if (body["team_blue"].t() == crow::json::type::True)
                {
                    latest_data.team_blue = true;
                    lcm_control.team_blue_from_lcm = false;
                    std::cout << "[POST] team_blue atualizado manualmente para azul\n";
                }
                else if (body["team_blue"].t() == crow::json::type::False)
                {
                    latest_data.team_blue = false;
                    lcm_control.team_blue_from_lcm = false;
                    std::cout << "[POST] team_blue atualizado manualmente para amarelo\n";
                }
                else
                {
                    std::cerr << "[ERRO] Valor inválido para team_blue (esperado booleano)\n";
                }
            }

            // Atualiza goalkeeper_id manualmente e desativa fonte LCM
            if (body.has("goalkeeper_id") && body["goalkeeper_id"].t() == crow::json::type::Number)
            {
                int id = body["goalkeeper_id"].i();
                lcm_control.goalkeeper_id_from_lcm = false; // bloqueia updates pelo LCM

                if (latest_data.team_blue)
                    latest_data.blue.goalkeeper_id = id;
                else
                    latest_data.yellow.goalkeeper_id = id;

                std::cout << "[POST] goalkeeper_id atualizado manualmente para " << id << std::endl;
            }

            // ---- Flags booleanas ----
            if (body.has("ssl_vision") && (body["ssl_vision"].t() == crow::json::type::True || body["ssl_vision"].t() == crow::json::type::False)) {
                latest_data.ssl_vision = body["ssl_vision"].b();
                std::cout << "[POST] ssl_vision atualizado para " << (latest_data.ssl_vision ? "true" : "false") << std::endl;
            }
            if (body.has("autoreferee") && (body["autoreferee"].t() == crow::json::type::True || body["autoreferee"].t() == crow::json::type::False)) {
                latest_data.autoreferee = body["autoreferee"].b();
                std::cout << "[POST] autoreferee atualizado para " << (latest_data.autoreferee ? "true" : "false") << std::endl;
            }
            if (body.has("competition_mode") && (body["competition_mode"].t() == crow::json::type::True || body["competition_mode"].t() == crow::json::type::False)) {
                latest_data.competition_mode = body["competition_mode"].b();
                std::cout << "[POST] competition_mode atualizado para " << (latest_data.competition_mode ? "true" : "false") << std::endl;
            }
            if (body.has("bool_controller") && (body["bool_controller"].t() == crow::json::type::True || body["bool_controller"].t() == crow::json::type::False)) {
                latest_data.bool_controller = body["bool_controller"].b();
                std::cout << "[POST] bool_controller atualizado para " << (latest_data.bool_controller ? "true" : "false") << std::endl;
            }
            if (body.has("debug_mode") && (body["debug_mode"].t() == crow::json::type::True || body["debug_mode"].t() == crow::json::type::False)) {
                latest_data.debug_mode = body["debug_mode"].b();
                std::cout << "[POST] debug_mode atualizado para " << (latest_data.debug_mode ? "true" : "false") << std::endl;
            }
            if (body.has("half_field") && (body["half_field"].t() == crow::json::type::True || body["half_field"].t() == crow::json::type::False)) {
                latest_data.half_field = body["half_field"].b();
                std::cout << "[POST] half_field atualizado para " << (latest_data.half_field ? "true" : "false") << std::endl;
            }
            if (body.has("iris_as_GC") && (body["iris_as_GC"].t() == crow::json::type::True || body["iris_as_GC"].t() == crow::json::type::False)) {
                latest_data.iris_as_GC = body["iris_as_GC"].b();
                std::cout << "[POST] iris_as_GC atualizado para " << (latest_data.iris_as_GC ? "true" : "false") << std::endl;
            }

            // ---- Portas ----
            if (body.has("stm_port") && body["stm_port"].t() == crow::json::type::Number) {
                latest_data.stm_port = body["stm_port"].i();
                std::cout << "[POST] stm_port atualizado para " << latest_data.stm_port << std::endl;
            }
            if (body.has("mcast_port_gc") && body["mcast_port_gc"].t() == crow::json::type::Number) {
                latest_data.mcast_port_gc = body["mcast_port_gc"].i();
                std::cout << "[POST] mcast_port_gc atualizado para " << latest_data.mcast_port_gc << std::endl;
            }
            if (body.has("mcast_port_vision_sslvision") && body["mcast_port_vision_sslvision"].t() == crow::json::type::Number) {
                latest_data.mcast_port_vision_sslvision = body["mcast_port_vision_sslvision"].i();
                std::cout << "[POST] mcast_port_vision_sslvision atualizado para " << latest_data.mcast_port_vision_sslvision << std::endl;
            }
            if (body.has("mcast_port_vision_grsim") && body["mcast_port_vision_grsim"].t() == crow::json::type::Number) {
                latest_data.mcast_port_vision_grsim = body["mcast_port_vision_grsim"].i();
                std::cout << "[POST] mcast_port_vision_grsim atualizado para " << latest_data.mcast_port_vision_grsim << std::endl;
            }
            if (body.has("mcast_port_vision_tracked") && body["mcast_port_vision_tracked"].t() == crow::json::type::Number) {
                latest_data.mcast_port_vision_tracked = body["mcast_port_vision_tracked"].i();
                std::cout << "[POST] mcast_port_vision_tracked atualizado para " << latest_data.mcast_port_vision_tracked << std::endl;
            }
            if (body.has("cams_number") && body["cams_number"].t() == crow::json::type::Number) {
                latest_data.cams_number = body["cams_number"].i();
                std::cout << "[POST] cams_number atualizado para " << latest_data.cams_number << std::endl;
            }

            // ---- Publicar mensagem no LCM ----
            data::tartarus msg;
            msg.ssl_vision = latest_data.ssl_vision;
            msg.autoreferee = latest_data.autoreferee;
            msg.competition_mode = latest_data.competition_mode;
            msg.bool_controller = latest_data.bool_controller;
            msg.debug_mode = latest_data.debug_mode;
            msg.half_field = latest_data.half_field;
            msg.iris_as_GC = latest_data.iris_as_GC;
            msg.goalkeeper_id = latest_data.team_blue ? latest_data.blue.goalkeeper_id : latest_data.yellow.goalkeeper_id;
            msg.stm_port = latest_data.stm_port;
            msg.mcast_port_gc = latest_data.mcast_port_gc;
            msg.mcast_port_vision_grsim = latest_data.mcast_port_vision_grsim;
            msg.mcast_port_vision_sslvision = latest_data.mcast_port_vision_sslvision;
            msg.mcast_port_vision_tracked = latest_data.mcast_port_vision_tracked;
            msg.team_blue = latest_data.team_blue;
            msg.cams_number = latest_data.cams_number;

            global_lcm.publish("tartarus", &msg);
            std::cout << "[POST] Mensagem publicada no canal 'tartarus'\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao processar POST: " << e.what() << std::endl;
            return crow::response(500, "Erro interno");
        }

        crow::response res("{\"status\":\"ok\"}");
        res.code = 200;
        res.add_header("Content-Type", "application/json");
        return res;
    });

    app.port(5000).multithreaded().run();
    lcm_thread.join();
    return 0;
}
