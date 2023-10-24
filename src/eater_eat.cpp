#include "eater_eat.h"
#include "food.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

EaterEat::EaterEat() {}

EaterEat::~EaterEat() {}

void EaterEat::_bind_methods() {
    ClassDB::bind_method(D_METHOD("eat_food"), &EaterEat::eat_food);
}


void EaterEat::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    eater = Object::cast_to<Eater>(this->get_parent()->get_parent());
}

void EaterEat::enter() {
    UtilityFunctions::print("enter eater eat state");
    eat_food();
}

void EaterEat::update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    
}

void EaterEat::physics_update(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void EaterEat::eat_food() {
    if (curr_food) {
        UtilityFunctions::print("calling play_eat()");
        eater->play_eat();
        curr_food->set_position(Vector3(rand.randf_range(-150, 150), rand.randf_range(4, 20), 
            rand.randf_range(-150, 150)));
    }
    emit_signal("transitioned", "eatereat", "eaterchase");
}

void EaterEat::exit() {
    UtilityFunctions::print("exit eater eat state");
}

void EaterEat::set_curr_food(Food *p_curr_food) {
    curr_food = p_curr_food;
}