#include "eater_retreat.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterRetreat::EaterRetreat() {}

EaterRetreat::~EaterRetreat() {}

void EaterRetreat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    player = get_node<Player>("../../../Player");
    eater = Object::cast_to<Eater>(this->get_parent()->get_parent());
    raycast1 = get_node<Raycast>("../../Raycast");
    raycast2 = get_node<Raycast>("../../Raycast2");
    raycast3 = get_node<Raycast>("../../Raycast3");
    raycast4 = get_node<Raycast>("../../Raycast4");
}

void EaterRetreat::enter() {
    UtilityFunctions::print("enter eater retreat state");
}

void EaterRetreat::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void EaterRetreat::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if ((raycast1->is_colliding() && raycast1->get_collider() == player) ||
        (raycast2->is_colliding() && raycast2->get_collider() == player) ||
        (raycast3->is_colliding() && raycast3->get_collider() == player) ||
        (raycast4->is_colliding() && raycast4->get_collider() == player)) {
        eater->set_position(Vector3(0, 10, 0));
    } else {
        if (player->is_inside_tree() && eater) {
            Vector3 dest = player->get_position();
            Vector3 dir = -1 * (dest - eater->get_position());
            dir.normalize();
            eater->set_velocity(dir * 500 * delta);
            eater->move_and_slide();
            eater->set_position(eater->get_position());
            if ((dest - eater->get_position()).length() > 40) {
                emit_signal("transitioned", "eaterretreat", "eaterchase");
            }
        }
    }
}

void EaterRetreat::exit() {
    UtilityFunctions::print("exit eater retreat state");
}