#include "state.h"
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

void State::_bind_methods() {
    ADD_SIGNAL(MethodInfo("transitioned"));
}

State::State() {}

State::~State() {}

void State::_ready() {}

void State::enter() {
    UtilityFunctions::print("enter generic state");
}

void State::exit() {
    UtilityFunctions::print("exit generic state");
}

void State::update(double delta) {
    UtilityFunctions::print("generic update");
}

void State::physics_update(double delta) {}