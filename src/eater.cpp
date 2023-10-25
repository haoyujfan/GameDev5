#include "eater.h"
#include "camera.h"
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
#include <godot_cpp/classes/a_star3d.hpp>

#include <cstdlib>

using namespace godot;

void Eater::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_slide_angle"), &Eater::get_slide_angle);
    ClassDB::bind_method(D_METHOD("set_slide_angle", "slide angle"), &Eater::set_slide_angle);
    ClassDB::add_property("Eater", PropertyInfo(Variant::FLOAT, "slide angle", PROPERTY_HINT_RANGE, 
        "0.05,1.0, 0.01"), "set_slide_angle", "get_slide_angle");

    ClassDB::bind_method(D_METHOD("get_jump_force"), &Eater::get_jump_force);
    ClassDB::bind_method(D_METHOD("set_jump_force", "jump force"), &Eater::set_jump_force);
    ClassDB::add_property("Eater", PropertyInfo(Variant::FLOAT, "jump force", PROPERTY_HINT_RANGE, 
        "100, 500, 50"), "set_jump_force", "get_jump_force");

    ClassDB::bind_method(D_METHOD("get_glide_gravity"), &Eater::get_glide_gravity);
    ClassDB::bind_method(D_METHOD("set_glide_gravity", "glide gravity"), &Eater::set_glide_gravity);
    ClassDB::add_property("Eater", PropertyInfo(Variant::FLOAT, "glide gravity", PROPERTY_HINT_RANGE, 
        "100, 600, 50"), "set_glide_gravity", "get_glide_gravity");

    ClassDB::bind_method(D_METHOD("get_gravity"), &Eater::get_gravity);
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &Eater::set_gravity);
    ClassDB::add_property("Eater", PropertyInfo(Variant::FLOAT, "gravity", PROPERTY_HINT_RANGE, 
        "500, 2000, 100"), "set_gravity", "get_gravity");

    ClassDB::bind_method(D_METHOD("get_air_resistance"), &Eater::get_air_resistance);
    ClassDB::bind_method(D_METHOD("set_air_resistance", "air_resistance"), &Eater::set_air_resistance);
    ClassDB::add_property("Eater", PropertyInfo(Variant::FLOAT, "air resistance", PROPERTY_HINT_RANGE, 
        "0, 2, 0.1"), "set_air_resistance", "get_air_resistance");

    ADD_SIGNAL(MethodInfo("interact_orange"));
    ADD_SIGNAL(MethodInfo("sound_effect_toggle", PropertyInfo(Variant::STRING, "toggle")));
    ADD_SIGNAL(MethodInfo("rotate_mode_toggle", PropertyInfo(Variant::STRING, "toggle")));
}

Eater::Eater() {
    gravity = 1400.0;
    glide_gravity = 200.0;
    jump_velocity = 300.0;
    speed = 2;
    air_resistance = 0;
    current_air = 0;
    velocity = Vector3(0.0, 0.0, 0.0);
    position = Vector3(rand.randf_range(-150, 150), 6, rand.randf_range(-150, 150));
}

Eater::~Eater() {}

void Eater::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    initialize_sound();
    set_position(position);
}

void Eater::_process(double delta) {
    play_eat();
}

void Eater::_physics_process(double delta) {}

void Eater::initialize_sound() {
    String squish_path = "res://audio/squish.mp3";
    Ref<FileAccess> squish_file = FileAccess::open(squish_path, FileAccess::ModeFlags::READ);
    FileAccess *squish_ptr = Object::cast_to<FileAccess>(*squish_file);
    eat = memnew(AudioStreamMP3);
    eat->set_data(squish_ptr->get_file_as_bytes(squish_path));
    eat_player = get_node<AudioStreamPlayer>("EatPlayer");
    if (eat_player) {
        UtilityFunctions::print("Got eat_player");
    }
}

void Eater::play_eat() {
    if (eat_player && !Engine::get_singleton()->is_editor_hint()) {
        eat_player->set_stream(eat);
        eat_player->set_volume_db(-12.0);
        eat_player->play(0.0);
    }
}

void Eater::food_interaction(bool entered) {}

void Eater::set_gravity(float p_gravity) {
    gravity = p_gravity;
}

float Eater::get_gravity() {
    return gravity;
}

void Eater::set_slide_angle(float p_angle) {
    set_floor_max_angle(p_angle);
}

float Eater::get_slide_angle() {
    return get_floor_max_angle();
}

void Eater::set_jump_force(float p_force) {
    jump_velocity = p_force;
}

float Eater::get_jump_force() {
    return jump_velocity;
}

void Eater::set_glide_gravity(float p_glide_gravity) {
    glide_gravity = p_glide_gravity;
}

float Eater::get_glide_gravity() {
    return glide_gravity;
}

void Eater::set_air_resistance(float p_air_resistance) {
    air_resistance = p_air_resistance;
}

float Eater::get_air_resistance() {
    return air_resistance;
}