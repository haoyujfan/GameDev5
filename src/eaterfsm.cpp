#include "eaterfsm.h"
#include <godot_cpp/variant/utility_functions.hpp>

#include <cstdlib>

using namespace godot;

void Eaterfsm::_bind_methods() {}

Eaterfsm::Eaterfsm() {}

Eaterfsm::~Eaterfsm() {}

void Eaterfsm::_ready() {
    // states["name"] = get_node<>("");
}

void Eaterfsm::_process(double delta) {
    current_state.Update(delta);
}

void Eaterfsm::_physics_process(double delta) {
    current_state.Physics_Update(delta);
}