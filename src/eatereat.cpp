#include "eatereat.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

void EaterEat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    eater = get_node<CharacterBody3D>("../../../Eater");
}

void EaterEat::Enter() {
    UtilityFunctions::print("enter eat state");
}

void EaterEat::Update(double delta) {
    eater->set_velocity(Vector3(10, 10, 10) * delta);
    eater->move_and_slide();
    UtilityFunctions::print("in update of eat state");
}

void EaterEat::Physics_Update(double delta) {

}

void EaterEat::Exit() {
    UtilityFunctions::print("exit eat state");
}