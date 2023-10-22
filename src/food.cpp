#include "food.h"
#include "ground.h"
#include "eater_eat.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <iostream>
#include <stdlib.h>


using namespace godot;

// bind c++ methods to godot
void Food::_bind_methods() {
    ClassDB::bind_method(D_METHOD("food_body_entered", "node"), &Food::food_body_entered);
    ClassDB::bind_method(D_METHOD("food_body_exited", "node"), &Food::food_body_exited);

    ADD_SIGNAL(MethodInfo("eater_ate", PropertyInfo(Variant::STRING, "food_name")));

    
}

// constructor
Food::Food() {}

// destructor
Food::~Food() {}

void Food::_process(double delta) {}

// initialize the food when its children are ready 
void Food::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    EaterEat *eater_eat = get_node<EaterEat>("../Eater/EaterFSM/EaterEat");
    entered = false;
    enter_class = "";
    this->connect("body_entered", Callable(this, "food_body_entered"));
    this->connect("body_exited", Callable(this, "food_body_exited"));
    this->connect("eater_ate", Callable(eater_eat, "eat_food"));
}

void Food::food_body_entered(const Node3D* node) {
    if (node->get_class() == "Player" || node->get_class() == "Eater") {
        entered = true;
        enter_class = node->get_class();
        if (enter_class == "Eater") {
            emit_signal("eater_ate", this->get_name());
        }
        else {
            entered_by_player = true;
        }
    }
}

void Food::food_body_exited(const Node3D* node) {
    if (node->get_class() == "Player" || node->get_class() == "Eater") {
        entered = false;
        enter_class = "";
        if (node->get_class() == "Player") {
            entered_by_player = false;
        }
    }
}

bool Food::is_entered() {
    return entered;
}

bool Food::is_entered_by_player() {
    return entered_by_player;
}

String Food::get_enter_class() {
    return enter_class;
}