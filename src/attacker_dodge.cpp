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
}

void AttackerDodge::exit() {
    UtilityFunctions::print("exit attacker dodge state");
}