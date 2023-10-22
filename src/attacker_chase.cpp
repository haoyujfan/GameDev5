#include "attacker_chase.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/a_star3d.hpp>


#include <cstdlib>

using namespace godot;

AttackerChase::AttackerChase() {}

AttackerChase::~AttackerChase() {}

void AttackerChase::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    a_star = memnew(AStar3D);
}

void AttackerChase::enter() {
    UtilityFunctions::print("enter attacker chase state");
}

void AttackerChase::update(double delta) {}

void AttackerChase::physics_update(double delta) {}

void AttackerChase::exit() {
    UtilityFunctions::print("exit attacker chase state");
}