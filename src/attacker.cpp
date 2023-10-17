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
#include <godot_cpp/classes/scene_tree.hpp>
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
    position = Vector3(10.0, 10.0, 0.0);
}

Attacker::~Attacker() {}

void Attacker::_ready() {
    momentum = Vector3(0.0, 0.0, 0.0);
    set_position(position);
    ray1 = get_node<Raycast>("Raycast");
    ray2 = get_node<Raycast>("Raycast2");
    ray3 = get_node<Raycast>("Raycast3");
    ray4 = get_node<Raycast>("Raycast4");
    colliding = NULL;

    a_star = memnew(AStar3D);

    player = get_node<Player>("../Player");

    tree = get_tree();
}

void Attacker::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void Attacker::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Vector3 dest = player->get_position();
    Vector3 dir = dest - position;
    dir.normalize();
    velocity = dir * 500 * delta;
    position = get_position();

    // // ledge stop
    // if (ray1->is_colliding() && ray2->is_colliding() &&
    //     ray3->is_colliding() && ray4->is_colliding()) {
    //     // WASD movement
    //     momentum = movement();
    // } else {
    //     gravity = 0;
    //     velocity.y = 0;
    // }
    set_velocity(velocity);
    move_and_slide();
}

Vector3 Attacker::movement() {
    Vector3 result = Vector3(0, 0, 0);
    if (result == Vector3(0, 0, 0)) {
        return momentum;
    } else {
        return result;
    }
}



void Attacker::ledge_stop() {
//      // ledge stop
//     if (ray1->is_colliding() && ray2->is_colliding() &&
//         ray3->is_colliding() && ray4->is_colliding()) {
//         momentum = movement();
//     }
}

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
