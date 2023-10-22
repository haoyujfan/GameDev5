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
    eater = get_node<Eater>("../../../Eater");
    a_star = memnew(AStar3D);
    food1 = get_node<Food>("../../../Food");
    food2 = get_node<Food>("../../../Food2");
    food3 = get_node<Food>("../../../Food3");
    food4 = get_node<Food>("../../../Food4");
}

void EaterChase::enter() {
    UtilityFunctions::print("enter chase state");
}

void EaterChase::update(double delta) {}

void EaterChase::physics_update(double delta) {
     if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (food1 && food2 && food3 && food4) {
        a_star->add_point(1, food1->get_position());
        a_star->add_point(2, food2->get_position());
        a_star->add_point(3, food3->get_position());
        a_star->add_point(4, food4->get_position());
    }
    int id = a_star->get_closest_point(eater->get_position());
    Vector3 dest = a_star->get_point_position(id);
    Vector3 dir = dest - eater->get_position();
    dir.normalize();
    //eater->velocity = dir * 500 * delta;
    eater->set_velocity(dir * 500 * delta);
    eater->move_and_slide();
    eater->set_position(eater->get_position());
    if ((eater->get_position() - dest).length() < 5) {
        emit_signal("transitioned", "eaterchase", "eatereat");
    }
}

void EaterChase::exit() {
    UtilityFunctions::print("exit chase state");
}