#include "state.h"

using namespace godot;

void State::_bind_methods() {
    ADD_SIGNAL(MethodInfo("transitioned"));
}

State::State() {}

State::~State() {}

void State::enter() {}

void State::exit() {}

void State::update(double delta) {}

void State::physics_update(double delta) {}