#include "finite_state_machine.h"
#include "state.h"
#include <godot_cpp/variant/utility_functions.hpp>


#include <cstdlib>

using namespace godot;

void FiniteStateMachine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_child_transition", "old_state_name", "new_state_name"), &FiniteStateMachine::on_child_transition);
}

FiniteStateMachine::FiniteStateMachine() {}

FiniteStateMachine::~FiniteStateMachine() {}

void FiniteStateMachine::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    TypedArray children = get_children();
    initial_state = Object::cast_to<State>(children[0]); // get first child state
    for (int i = 0; i < children.size(); i++) {
        Variant child = children[i];
        State *child_state = Object::cast_to<State>(child);
        // if (child_state->get_name().to_lower() == "eaterchase") {
        //     initial_state = child_state;
        // }
        states[child_state->get_name().to_lower()] = child;
        child_state->connect("transitioned", Callable(this, "on_child_transition"));
    }
    if (initial_state) {
        initial_state->enter();
        current_state = initial_state;
    }

}

void FiniteStateMachine::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (current_state) {
        current_state->update(delta);
    }
}

void FiniteStateMachine::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (current_state) {
        current_state->physics_update(delta);
    }
}

void FiniteStateMachine::on_child_transition(String old_state_name, String new_state_name) {
    if (old_state_name.to_lower() != current_state->get_name().to_lower()) {
        return;
    }

    Variant new_state = states.get(new_state_name.to_lower(), nullptr);

    if (!new_state) {
        return;
    }
    if (current_state) {
        current_state->exit();
    }
    Object::cast_to<State>(new_state)->enter();

    current_state = Object::cast_to<State>(new_state);
}