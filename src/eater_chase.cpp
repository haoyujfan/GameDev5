#include "eater_chase.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterChase::EaterChase() {}

EaterChase::~EaterChase() {}

void EaterChase::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    eater = get_node<Eater>("../../../Eater");
}

void EaterChase::enter() {
    UtilityFunctions::print("enter chase state");
}

void EaterChase::update(double delta) {
    if (eater) {
        eater->set_velocity(Vector3(0, 10, 0));
        emit_signal("transitioned", "EaterChase", "EaterEat");
    }
}

void EaterChase::physics_update(double delta) {

}

void EaterChase::exit() {
    UtilityFunctions::print("exit chase state");
}