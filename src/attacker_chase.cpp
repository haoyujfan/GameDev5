#include "attacker_chase.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

AttackerChase::AttackerChase() {}

AttackerChase::~AttackerChase() {}

void AttackerChase::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    attacker = Object::cast_to<Attacker>(this->get_parent()->get_parent());
    player = get_node<Player>("../../../Player");
    a_star = memnew(AStar3D);
    raycast1 = get_node<Raycast>("../../Raycast");
    raycast2 = get_node<Raycast>("../../Raycast2");
    raycast3 = get_node<Raycast>("../../Raycast3");
    raycast4 = get_node<Raycast>("../../Raycast4");
    nav = get_node<Navigation>("../../Navigation");
}

void AttackerChase::enter() {
    UtilityFunctions::print("enter attacker chase state");
}

void AttackerChase::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void AttackerChase::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if ((raycast1->is_colliding() && raycast1->get_collider() == player) ||
        (raycast2->is_colliding() && raycast2->get_collider() == player) ||
        (raycast3->is_colliding() && raycast3->get_collider() == player) ||
        (raycast4->is_colliding() && raycast4->get_collider() == player)) {
        nav->teleport(attacker, Vector3(0, 10, 0));
    } else {
        Vector3 dest = player->get_position();
        nav->chase(attacker, dest);
        if ((dest - attacker->get_position()).length() < 11) {
            emit_signal("transitioned", "attackerchase", "attackerattack");
        }
    }
}

void AttackerChase::exit() {
    UtilityFunctions::print("exit attacker chase state");
}