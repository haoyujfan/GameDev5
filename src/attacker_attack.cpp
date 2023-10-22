#include "attacker_attack.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

AttackerAttack::AttackerAttack() {}

AttackerAttack::~AttackerAttack() {}

void AttackerAttack::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    attacker = get_node<Attacker>("../../../Attacker");
    player = get_node<Player>("../../../Player");
    a_star = memnew(AStar3D);
}

void AttackerAttack::enter() {
    UtilityFunctions::print("enter attacker attack state");
}

void AttackerAttack::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    player->set_lives(player->get_lives() - 1);
    player->life_lost_GUI();
    emit_signal("transitioned", "attackerattack", "attackerchase");
}

void AttackerAttack::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void AttackerAttack::exit() {
    UtilityFunctions::print("exit attacker attack state");
}