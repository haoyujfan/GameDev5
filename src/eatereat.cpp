#include "eatereat.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterEat::EaterEat() {}

EaterEat::~EaterEat() {}

void EaterEat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    eater = get_node<CharacterBody3D>("../../../Eater");
}

void EaterEat::enter() {
    UtilityFunctions::print("enter eat state");
}

void EaterEat::update(double delta) {
    eater->set_velocity(Vector3(10, 10, 10) * delta);
    eater->move_and_slide();
    UtilityFunctions::print("in update of eat state");
}

void EaterEat::physics_update(double delta) {

}

void EaterEat::exit() {
    UtilityFunctions::print("exit eat state");
}