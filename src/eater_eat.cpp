#include "eater_eat.h"
#include "food.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterEat::EaterEat() {}

EaterEat::~EaterEat() {}

void EaterEat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    eater = get_node<Eater>("../../../Eater");
}

void EaterEat::enter() {
    UtilityFunctions::print("enter eat state");
}

void EaterEat::update(double delta) {
    if (eater) {
        
        emit_signal("transitioned", "eatereat", "eaterchase");
    }
}

void EaterEat::physics_update(double delta) {

}

void EaterEat::eat_food(String food_name) {
    Food *curr_food = get_node<Food>("../../../" + food_name);
}

void EaterEat::exit() {
    UtilityFunctions::print("exit eat state");
}