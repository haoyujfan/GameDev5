#include "eater_fsm.h"
#include "state.h"
#include <godot_cpp/variant/utility_functions.hpp>

#include <cstdlib>

using namespace godot;

void EaterFSM::_bind_methods() {}

EaterFSM::EaterFSM() {
    // states = *memnew(Dictionary());
}

EaterFSM::~EaterFSM() {}

void EaterFSM::_ready() {
    TypedArray children = get_children();
    for (int i = 0; i < children.size(); i++) {
        Variant child = children[i];
        State child_state = *Object::cast_to<State>(child);
        states[child_state.get_name().to_lower()] = child;
        child_state.connect("transitioned", Callable(this, "on_child_transition"));
    }
    if (initial_state) {
        initial_state->enter();
        current_state = initial_state;
    }

}

void EaterFSM::_process(double delta) {
    if (current_state) {
        current_state->update(delta);
    }
}

void EaterFSM::_physics_process(double delta) {
    if (current_state) {
        current_state->physics_update(delta);
    }
}

void EaterFSM::on_child_transition(State *p_old_state, String new_state_name) {
    if (p_old_state != current_state) {
        return;
    }
    Variant new_state = states.get(new_state_name.to_lower(), NULL);
}