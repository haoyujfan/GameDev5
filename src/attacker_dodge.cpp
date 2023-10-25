#include "attacker_dodge.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

AttackerDodge::AttackerDodge() {}

AttackerDodge::~AttackerDodge() {}

void AttackerDodge::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    a_star = memnew(AStar3D);
    attacker = Object::cast_to<Attacker>(this->get_parent()->get_parent());
    player = get_node<Player>("../../../Player");
    raycast1 = get_node<Raycast>("../../Raycast");
    raycast2 = get_node<Raycast>("../../Raycast2");
    raycast3 = get_node<Raycast>("../../Raycast3");
    raycast4 = get_node<Raycast>("../../Raycast4");
    nav = get_node<Navigation>("../../Navigation");
}

void AttackerDodge::enter() {
    UtilityFunctions::print("enter attacker dodge state");
}

void AttackerDodge::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void AttackerDodge::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if ((raycast1->is_colliding() && raycast1->get_collider() == player) ||
        (raycast2->is_colliding() && raycast2->get_collider() == player) ||
        (raycast3->is_colliding() && raycast3->get_collider() == player) ||
        (raycast4->is_colliding() && raycast4->get_collider() == player)) {
        nav->teleport(attacker, Vector3(0, 10, 0));
    } else {
        if (player->is_inside_tree() && attacker) {
            Vector3 dest = player->get_position();
            Vector3 dir = -1 * (dest - attacker->get_position());
            dir.normalize();
            attacker->set_velocity(dir * 500 * delta);
            attacker->move_and_slide();
            attacker->set_position(attacker->get_position());
            if ((dest - attacker->get_position()).length() > 40) {
                emit_signal("transitioned", "attackerdodge", "attackerchase");
            }
        }
    }
}

void AttackerDodge::exit() {
    UtilityFunctions::print("exit attacker dodge state");
}