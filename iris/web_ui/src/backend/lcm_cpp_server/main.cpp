#define CROW_MAIN
#include "crow_all.h"

#include <lcm/lcm-cpp.hpp>
#include <mutex>
#include <thread>
#include <iostream>

// Ajuste os includes para o caminho correto das mensagens LCM geradas (.hpp)
#include "../../../../../data_lcm/data/game_controller.hpp"
#include "../../../../../data_lcm/data/vision.hpp"
#include "../../../../../data_lcm/data/tartarus.hpp"
#include "../../../../../data_lcm/data/ia.hpp"

std::mutex data_mutex;

struct LatestData {
    int64_t timestamp = 0;
    int robots_size = 0;
    bool team_blue = false;
    int score_yellow = 0;
    int score_blue = 0;
    float field_length = 0.0f;
    std::string processo;
    std::string estrategia;
    bool ssl_vision = false;
    bool team_blue_status = false;
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
        latest_data.score_blue = msg_GC.blue.score;
        latest_data.score_yellow = msg_GC.yellow.score;
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
    }

    void handleIA(const lcm::ReceiveBuffer*, const std::string&, const ia_t* msg) {
        std::lock_guard<std::mutex> lock(data_mutex);
        msg_ia = *msg;
        latest_data.robots_size = msg_ia.robots_size;
        latest_data.processo = msg_ia.processo;
        latest_data.estrategia = msg_ia.estrategia;
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

    while (lc.handle() == 0) {
        // Processa mensagens LCM
    }
}

int main() {
    std::thread lcm_thread(lcm_thread_func);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/data")([] {
        std::lock_guard<std::mutex> lock(data_mutex);
        crow::json::wvalue res;
        res["timestamp"] = latest_data.timestamp;
        res["robots_size"] = latest_data.robots_size;
        res["team_blue"] = latest_data.team_blue;
        res["score_yellow"] = latest_data.score_yellow;
        res["score_blue"] = latest_data.score_blue;
        res["field_length"] = latest_data.field_length;
        res["processo"] = latest_data.processo;
        res["estrategia"] = latest_data.estrategia;
        res["ssl_vision"] = latest_data.ssl_vision;
        res["team_blue_status"] = latest_data.team_blue_status;
        return res;
    });

    app.port(5000).multithreaded().run();

    lcm_thread.join();

    return 0;
}
