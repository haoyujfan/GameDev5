#include "attacker_attack.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

void AttackerAttack::_bind_methods() {
    ADD_SIGNAL(MethodInfo("hit_player"));
}

AttackerAttack::AttackerAttack() {}

AttackerAttack::~AttackerAttack() {}

void AttackerAttack::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    attacker = Object::cast_to<Attacker>(this->get_parent()->get_parent());
    player = get_node<Player>("../../../Player");
    this->connect("hit_player", Callable(player, "play_hurt"));
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
    player->set_is_hurt(true);
    player->set_hurt_frames(0);
    player->life_lost_GUI();
    emit_signal("hit_player");
    emit_signal("transitioned", "attackerattack", "attackerdodge");
}

void AttackerAttack::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void AttackerAttack::exit() {
    UtilityFunctions::print("exit attacker attack state");
}