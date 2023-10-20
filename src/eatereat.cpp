#include "eatereat.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

void Eatereat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    eater = get_node<CharacterBody3D>("../../../Eater");
}

void Eatereat::Enter() {
    UtilityFunctions::print("enter eat state");
}

void Eatereat::Update(double delta) {
    eater->set_velocity(Vector3(10, 10, 10) * delta);
    UtilityFunctions::print("in update of eat state");
}

void Eatereat::Physics_Update(double delta) {

}

void Eatereat::Exit() {
    UtilityFunctions::print("exit eat state");
}