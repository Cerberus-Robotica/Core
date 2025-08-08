//
// Created by caiom on 4/4/2025.
//

#include "Leader.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <unordered_set>

#include "../include/handlers.hpp"
#include "TeamInfo.h"
#include "plays/PlayOurKickOff.h"

void Leader::start() {
    loop();
}

void Leader::loop() {
    std::cout << "Leader inicializado" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto t0 = std::chrono::steady_clock::now();
    receive_vision();
    delta_time = 0.016;

    while (true) {
        if (last_time_stamp == han.new_ia.timestamp) {
            continue;
        }
        auto t1 = std::chrono::steady_clock::now();

        receive_config();
        receive_vision();
        receive_gamecontroller();
        receive_field_geometry();

        inspect_enemy_team();
        world_analysis();
        select_plays();
        //imprimir_ativos();

        //std::cout << team.central_line_x << std::endl;

        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        delta_time = delta.count();
        if (delta_time == 0 or delta_time < 0) {
            std::cout << "??" << std::endl;
            delta_time = 1/60;
        }

    }

}

void Leader::receive_vision() {
    std::unordered_set<int> allies_detected = {};
    std::unordered_set<int> enemies_detected = {};

    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team.color == TeamInfo::blue) {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(Robot(i));
                }
            }
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = blue_robot.position_x;
            world.allies[rb_id].pos[1] = blue_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(Robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = blue_robot.position_x;
            world.enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (team.color == TeamInfo::yellow) {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(Robot(i));
                }
            }
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = yellow_robot.position_x;
            world.allies[rb_id].pos[1] = yellow_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(Robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = yellow_robot.position_x;
            world.enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }

    for (int i = 0; i < size(world.allies); i++) {
        if (allies_detected.find(i) != allies_detected.end()) world.allies[i].detected = true;
        else world.allies[i].detected = false;
    }

    for (int i = 0; i < size(world.enemies); i++) {
        if (enemies_detected.find(i) != enemies_detected.end()) world.enemies[i].detected = true;
        else world.enemies[i].detected = false;
    }

    if (delta_time != 0) {
        world.ball_speed[0] = (han.new_vision.balls.position_x - world.ball_pos[0])/(delta_time*1000);
        world.ball_speed[1] = (han.new_vision.balls.position_y - world.ball_pos[1])/(delta_time*1000);
    }
    world.ball_pos[0] = han.new_vision.balls.position_x;
    world.ball_pos[1] = han.new_vision.balls.position_y;

    last_time_stamp = han.new_vision.timestamp;
}

void Leader::receive_field_geometry() {
    //TODO implementar urgente
    team.striker_max_dislocation = fabs(world.their_defese_area[0][0] + world.their_defese_area[1][0]) / 2;
    team.mid_field_max_dislocation = std::min(fabs(world.their_defese_area[0][0]), fabs(world.their_defese_area[1][0]));
}

void Leader::receive_gamecontroller() {
    //TODO implementar maquina de estados dos estados do jogo
    team.current_command = TeamInfo::Command(han.new_GC.current_command);
    int is_team_blue = int(han.new_GC.team_blue);
    if (is_team_blue == 1) {
        team.color = TeamInfo::blue;
        team.goal_keeper_id = han.new_GC.blue.goalkeeper_id;
    }
    else if (is_team_blue == 0) {
        team.color = TeamInfo::yellow;
        team.goal_keeper_id = han.new_GC.yellow.goalkeeper_id;
    }
}

void Leader::receive_config() {
    //TODO receber time do tartarus ou do GC
    //TODO receber lado do time

    if (team.our_side == TeamInfo::right) team.our_side_sign = 1;
    else team.our_side_sign = -1;


}

void Leader::world_analysis() {
    world.ball_pos[0] != 0 ? team.central_line_x = world.ball_pos[0]
        : team.central_line_x = 0;
    //std::cout << team.central_line_x << std::endl;
}




void Leader::add_robot(int id) {
    if (id >= sizeof(team.active_robots)) {
        return;
    }
    if (team.active_robots[id] == 0) {
        team.active_robots[id] = 1;
        robots[id].start(&team);
    }
}

void Leader::select_plays() {
    // Contar aliados ativos
    int numOfActiveAllies = std::count_if(
        std::begin(team.active_robots),
        std::end(team.active_robots),
        [](int id) { return id != 0; }
    );

    struct PlayInfo {
        std::string name;
        int score;
        std::function<int(WorldModel&, TeamInfo&)> score_func;
        std::function<std::array<TeamInfo::role, 16>(
            WorldModel&, TeamInfo&, std::array<TeamInfo::role, 16>
        )> role_func;
    };

    std::array<PlayInfo, 4> plays = {{
        { "attack", 0,
          [&](WorldModel& w, TeamInfo& t) { return attack.score(w, t); },
          [&](WorldModel& w, TeamInfo& t, std::array<TeamInfo::role, 16> r) { return attack.role_assing(w, t, r); } },

        { "debug", 0,
          [&](WorldModel& w, TeamInfo& t) { return debug.score(w, t); },
          [&](WorldModel& w, TeamInfo& t, std::array<TeamInfo::role, 16> r) { return debug.role_assing(w, t, r); } },

        { "halt", 0,
          [&](WorldModel& w, TeamInfo& t) { return halt.score(w, t); },
          [&](WorldModel& w, TeamInfo& t, std::array<TeamInfo::role, 16> r) { return halt.role_assing(w, t, r); } },

        { "OurKickOff", 0,
        [&](WorldModel& w, TeamInfo& t) { return ourKickOff.score(w, t); },
        [&](WorldModel& w, TeamInfo& t, std::array<TeamInfo::role, 16> r) { return ourKickOff.role_assing(w, t, r); } }

    }};

    // Calcular scores
    for (auto& play : plays) {
        play.score = play.score_func(world, team);
    }

    // Ordenar do maior para o menor score
    std::sort(plays.begin(), plays.end(),
              [](const PlayInfo& a, const PlayInfo& b) {
                  return a.score > b.score;
              });

    // Criar lista inicial de roles
    std::array<TeamInfo::role, 16> roles;
    roles.fill(TeamInfo::unknown);

    // Aplicar roles da melhor play
    roles = plays.front().role_func(world, team, roles);

    // Copiar para o time
    team.roles = roles;
}


void Leader::inspect_enemy_team() {
    if (size(world.enemies) == 0) return;
    std::vector<int> active_enemies_ids = {};
    std::vector<double> distances_enemies_from_ball = {};
    if (size(world.enemies) == 0) return;
    for (int i = 0; i < size(world.enemies) ; i++) {
        if (world.enemies[i].detected) {
            active_enemies_ids.push_back(i);
            distances_enemies_from_ball.push_back(sqrt(pow(world.enemies[i].pos[0] - world.ball_pos[0],2) + pow(world.enemies[i].pos[1] - world.ball_pos[1],2)));
        }
    }
    if (team.color == TeamInfo::blue) {
        team.enemy_roles[han.new_GC.yellow.goalkeeper_id] = TeamInfo::goal_keeper;
    }
    else {
        team.enemy_roles[han.new_GC.blue.goalkeeper_id] = TeamInfo::goal_keeper;
    }
    unsigned int closest_idx = 0;
    for (int idx = 0; idx < active_enemies_ids.size(); idx++) {
        if (distances_enemies_from_ball[idx] < distances_enemies_from_ball[closest_idx]) {
                closest_idx = idx;
            }
    }
    unsigned int id = world.enemies[closest_idx].id;
    team.enemy_roles[id] = TeamInfo::striker;

}






void Leader::imprimir_ativos() {
    std::cout << std::endl << "[";
    for (int i = 0; i < 16 ; i++) {
        if (team.active_robots[i] == 1) {
            std::cout << i << ", ";
        }
    }
    std::cout << "]" << std::endl;
}