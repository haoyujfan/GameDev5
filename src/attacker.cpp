#include "attacker.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/classes/object.hpp>

#include <cstdlib>

using namespace godot;

void Attacker::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_slide_angle"), &Attacker::get_slide_angle);
    ClassDB::bind_method(D_METHOD("set_slide_angle", "slide angle"), &Attacker::set_slide_angle);
    ClassDB::add_property("Attacker", PropertyInfo(Variant::FLOAT, "slide angle", PROPERTY_HINT_RANGE, 
        "0.05,1.0, 0.01"), "set_slide_angle", "get_slide_angle");

    ClassDB::bind_method(D_METHOD("get_jump_force"), &Attacker::get_jump_force);
    ClassDB::bind_method(D_METHOD("set_jump_force", "jump force"), &Attacker::set_jump_force);
    ClassDB::add_property("Attacker", PropertyInfo(Variant::FLOAT, "jump force", PROPERTY_HINT_RANGE, 
        "100, 500, 50"), "set_jump_force", "get_jump_force");


    ClassDB::bind_method(D_METHOD("get_gravity"), &Attacker::get_gravity);
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &Attacker::set_gravity);
    ClassDB::add_property("Attacker", PropertyInfo(Variant::FLOAT, "gravity", PROPERTY_HINT_RANGE, 
        "500, 2000, 100"), "set_gravity", "get_gravity");

    ClassDB::bind_method(D_METHOD("get_air_resistance"), &Attacker::get_air_resistance);
    ClassDB::bind_method(D_METHOD("set_air_resistance", "air_resistance"), &Attacker::set_air_resistance);
    ClassDB::add_property("Attacker", PropertyInfo(Variant::FLOAT, "air resistance", PROPERTY_HINT_RANGE, 
        "0, 2, 0.1"), "set_air_resistance", "get_air_resistance");
}

Attacker::Attacker() {
    gravity = 1400.0;
    jump_velocity = 300.0;
    speed = 2;
    air_resistance = 0;
    current_air = 0;
    velocity = Vector3(0.0, 0.0, 0.0);
    position = Vector3(rand.randf_range(-150, 150), 6, rand.randf_range(-150, 150));
}

Attacker::~Attacker() {}

void Attacker::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    set_position(position);
}

void Attacker::_process(double delta) {}

void Attacker::_physics_process(double delta) {}

void Attacker::set_gravity(float p_gravity) {
    gravity = p_gravity;
}

float Attacker::get_gravity() {
    return gravity;
}

void Attacker::set_slide_angle(float p_angle) {
    set_floor_max_angle(p_angle);
}

float Attacker::get_slide_angle() {
    return get_floor_max_angle();
}

void Attacker::set_jump_force(float p_force) {
    jump_velocity = p_force;
}

float Attacker::get_jump_force() {
    return jump_velocity;
}


void Attacker::set_air_resistance(float p_air_resistance) {
    air_resistance = p_air_resistance;
}

float Attacker::get_air_resistance() {
    return air_resistance;
}
