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
    position = Vector3(-10.0, 10.0, 0.0);
}

Eater::~Eater() {}

void Eater::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    a_star = memnew(AStar3D);
    momentum = Vector3(0.0, 0.0, 0.0);
    set_position(position);
    initialize_sound();

    food1 = get_node<Food>("../Food");
    food2 = get_node<Food>("../Food2");
    food3 = get_node<Food>("../Food3");
    food4 = get_node<Food>("../Food4");

    // may crash in editor if food not loaded yet
    a_star->add_point(1, food1->get_position());
    a_star->add_point(2, food2->get_position());
    a_star->add_point(3, food3->get_position());
    a_star->add_point(4, food4->get_position());
    
    player = get_node<Player>("../Player");
}

void Eater::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    bool entered = food1->is_entered() || food2->is_entered() || 
        food3->is_entered() || food4->is_entered();
    
    // handle food interactions
    food_interaction(entered);
}

void Eater::_physics_process(double delta) {
    
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // sets rotate mode or strafe mode
    // reset gravity 
    if (this->is_on_floor()) {
        speed = 1;
        gravity = 1400.0;
        momentum = Vector3(0, 0, 0);
    }

    // gravity and jumping
    if (!this->is_on_floor()) {
        translate_object_local(momentum);
        velocity.y -= gravity * delta;
        speed -= air_resistance * delta;
    }

    int id = a_star->get_closest_point(position);
    Vector3 dest = a_star->get_point_position(id);
    Vector3 dir = dest - position;
    dir.normalize();
    velocity = dir * 500 * delta;
    
    set_velocity(velocity);
    move_and_slide();
    position = get_position();
}

void Eater::initialize_sound() {
    String squish_path = "res://audio/squish.mp3";
    Ref<FileAccess> squish_file = FileAccess::open(squish_path, FileAccess::ModeFlags::READ);
    FileAccess *squish_ptr = Object::cast_to<FileAccess>(*squish_file);
    interact = memnew(AudioStreamMP3);
    interact->set_data(squish_ptr->get_file_as_bytes(squish_path));
    interact_player = get_node<AudioStreamPlayer>("InteractPlayer");
}

void Eater::play_interact() {
    if (interact_player && !Engine::get_singleton()->is_editor_hint()) {
        interact_player->set_stream(interact);
        interact_player->set_volume_db(-12.0);
        interact_player->play(0.0);
    }
}

void Eater::food_interaction(bool entered) {
    if (entered) {
        // if (!interact_player->is_playing() && !player->get_sound_toggle()) {
        //         play_interact();
        // }
        if (food1->is_entered()) {
            food1->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 10), 
            rand.randf_range(-50, 50)));
            a_star->remove_point(1);
            a_star->add_point(1, food1->get_position());
        } 
        if (food2->is_entered()) {
            food2->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 10), 
            rand.randf_range(-50, 50)));
            a_star->remove_point(2);
            a_star->add_point(2, food2->get_position());
        } 
        if (food3->is_entered()) {
            food3->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 10), 
            rand.randf_range(-50, 50)));
            a_star->remove_point(3);
            a_star->add_point(3, food3->get_position());
        }
        if (food4->is_entered()) {
            food4->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 10), 
            rand.randf_range(-50, 50)));
            a_star->remove_point(4);
            a_star->add_point(4, food4->get_position());
        }
    }
}

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

