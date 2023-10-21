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
    eater = get_node<Eater>("../../../Eater");
}

void EaterRetreat::enter() {
    UtilityFunctions::print("enter retreat state");
}

void EaterRetreat::update(double delta) {
    if (eater) {
        eater->set_velocity(Vector3(0, 10, 0));
    }
}

void EaterRetreat::physics_update(double delta) {

}

void EaterRetreat::exit() {
    UtilityFunctions::print("exit retreat state");
}