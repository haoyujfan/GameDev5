#include "eater_eat.h"
#include "food.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterEat::EaterEat() {}

EaterEat::~EaterEat() {}

void EaterEat::_bind_methods() {
    ClassDB::bind_method(D_METHOD("eat_food", "food_name"), &EaterEat::eat_food);
}


void EaterEat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    eater = get_node<Eater>("../../../Eater");
}

void EaterEat::enter() {
    UtilityFunctions::print("enter eater eat state");
}

void EaterEat::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (curr_food) {
    curr_food->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 20), 
        rand.randf_range(-50, 50)));
    }
    emit_signal("transitioned", "eatereat", "eaterchase");
}

void EaterEat::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void EaterEat::eat_food(String food_name) {
    curr_food = get_node<Food>("../../../" + food_name);
}

void EaterEat::exit() {
    UtilityFunctions::print("exit eater eat state");
}