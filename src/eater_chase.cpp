#include "eater_chase.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

EaterChase::EaterChase() {}

EaterChase::~EaterChase() {}

void EaterChase::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    eater = Object::cast_to<Eater>(this->get_parent()->get_parent());
    a_star = memnew(AStar3D);
    food1 = get_node<Food>("../../../Food");
    food2 = get_node<Food>("../../../Food2");
    food3 = get_node<Food>("../../../Food3");
    food4 = get_node<Food>("../../../Food4");
    player = get_node<Player>("../../../Player");
    raycast1 = get_node<Raycast>("../../Raycast");
    raycast2 = get_node<Raycast>("../../Raycast2");
    raycast3 = get_node<Raycast>("../../Raycast3");
    raycast4 = get_node<Raycast>("../../Raycast4");
    nav = get_node<Navigation>("../../Navigation");
}

void EaterChase::enter() {
    UtilityFunctions::print("enter eater chase state");
}

void EaterChase::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void EaterChase::physics_update(double delta) {
     if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if ((raycast1->is_colliding() && raycast1->get_collider() == player) ||
        (raycast2->is_colliding() && raycast2->get_collider() == player) ||
        (raycast3->is_colliding() && raycast3->get_collider() == player) ||
        (raycast4->is_colliding() && raycast4->get_collider() == player)) {
        nav->teleport(eater, Vector3(0, 10, 0));
    } else {
        if (food1->is_inside_tree() && food2->is_inside_tree() && food3->is_inside_tree() && food4->is_inside_tree()) {
            a_star->add_point(1, food1->get_position());
            a_star->add_point(2, food2->get_position());
            a_star->add_point(3, food3->get_position());
            a_star->add_point(4, food4->get_position());
        }
        int id = a_star->get_closest_point(eater->get_position());
        
        Vector3 dir_p = Vector3(0.0, 0.0, 0.0);
        real_t dist_p = 1000.0;
        if (player->is_inside_tree()) {
            Vector3 dest_p = player->get_position();
            dir_p = dest_p - eater->get_position();
            dir_p.normalize();
            dist_p = (eater->get_position() - dest_p).length();
            if (dist_p < 20) {
                emit_signal("transitioned", "eaterchase", "eaterretreat");
            }
        }
        Vector3 dest = a_star->get_point_position(id);
        Vector3 dir = dest - eater->get_position();
        dir.normalize();
        eater->set_velocity(dir * 500 * delta);
        eater->move_and_slide();
        eater->set_position(eater->get_position());
        if ((eater->get_position() - dest).length() < 5) {
            emit_signal("transitioned", "eaterchase", "eatereat");
        }
    }
}

void EaterChase::exit() {
    UtilityFunctions::print("exit eater chase state");
}