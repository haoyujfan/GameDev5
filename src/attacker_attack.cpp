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
    a_star = memnew(AStar3D);
}

void AttackerAttack::enter() {
    UtilityFunctions::print("enter attacker attack state");
}

void AttackerAttack::update(double delta) {
    emit_signal("transitioned", "attackerattack", "attackerchase");
}

void AttackerAttack::physics_update(double delta) {}

void AttackerAttack::exit() {
    UtilityFunctions::print("exit attacker attack state");
}