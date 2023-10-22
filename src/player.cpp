#include "player.h"
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

void Player::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_slide_angle"), &Player::get_slide_angle);
    ClassDB::bind_method(D_METHOD("set_slide_angle", "slide angle"), &Player::set_slide_angle);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "slide angle", PROPERTY_HINT_RANGE, 
        "0.05,1.0, 0.01"), "set_slide_angle", "get_slide_angle");

    ClassDB::bind_method(D_METHOD("get_jump_force"), &Player::get_jump_force);
    ClassDB::bind_method(D_METHOD("set_jump_force", "jump force"), &Player::set_jump_force);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "jump force", PROPERTY_HINT_RANGE, 
        "100, 500, 50"), "set_jump_force", "get_jump_force");

    ClassDB::bind_method(D_METHOD("get_glide_gravity"), &Player::get_glide_gravity);
    ClassDB::bind_method(D_METHOD("set_glide_gravity", "glide gravity"), &Player::set_glide_gravity);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "glide gravity", PROPERTY_HINT_RANGE, 
        "100, 600, 50"), "set_glide_gravity", "get_glide_gravity");

    ClassDB::bind_method(D_METHOD("get_gravity"), &Player::get_gravity);
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &Player::set_gravity);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "gravity", PROPERTY_HINT_RANGE, 
        "500, 2000, 100"), "set_gravity", "get_gravity");

    ClassDB::bind_method(D_METHOD("get_air_resistance"), &Player::get_air_resistance);
    ClassDB::bind_method(D_METHOD("set_air_resistance", "air_resistance"), &Player::set_air_resistance);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "air resistance", PROPERTY_HINT_RANGE, 
        "0, 2, 0.1"), "set_air_resistance", "get_air_resistance");

    ClassDB::bind_method(D_METHOD("get_lives"), &Player::get_lives);

    ADD_SIGNAL(MethodInfo("interact_orange"));
    ADD_SIGNAL(MethodInfo("life_lost_attacker"));
    ADD_SIGNAL(MethodInfo("sound_effect_toggle", PropertyInfo(Variant::STRING, "toggle")));
    ADD_SIGNAL(MethodInfo("rotate_mode_toggle", PropertyInfo(Variant::STRING, "toggle")));
}

Player::Player() {
    lives = 0;
    gravity = 1400.0;
    glide_gravity = 200.0;
    jump_velocity = 300.0;
    speed = 2;
    air_resistance = 0;
    current_air = 0;
    velocity = Vector3(0.0, 0.0, 0.0);
    position = Vector3(0.0, 10.0, 0.0);
    hanging = false;
    AD_rotate = true;
    mute_sound_effects = false;
}

Player::~Player() {}

void Player::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    momentum = Vector3(0.0, 0.0, 0.0);
    set_position(position);
    initialize_sound();
    ray1 = get_node<Raycast>("Raycast");
    ray2 = get_node<Raycast>("Raycast2");
    ray3 = get_node<Raycast>("Raycast3");
    ray4 = get_node<Raycast>("Raycast4");
    camera_cast1 = get_node<Raycast>("Node3D/Camera/Raycast");
    camera_cast2 = get_node<Raycast>("Node3D/Camera/Raycast2");
    colliding = NULL;

    food1 = get_node<Food>("../Food");
    food2 = get_node<Food>("../Food2");
    food3 = get_node<Food>("../Food3");
    food4 = get_node<Food>("../Food4");

    camera = get_node<Camera>("Node3D/Camera");
    tree = get_tree();
}

void Player::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // handle food interactions
    bool entered = food1->is_entered() || food2->is_entered() || 
        food3->is_entered() || food4->is_entered();
    if (food1->get_enter_class() == "Player" || food2->get_enter_class() == "Player" ||
        food3->get_enter_class() == "Player" || food4->get_enter_class() == "Player") {
        food_interaction(entered);
    }

    // empty interaction
    if (!entered && Input::get_singleton()->is_action_just_pressed("E")) {
        if (!empty_interact_player->is_playing() && !mute_sound_effects) {
            play_empty_interact();
        }
    }
    // sound effect toggle
    if (Input::get_singleton()->is_action_just_pressed("Sound Effect")) {
        mute_sound_effects = !mute_sound_effects;
    }
    toggles();

    // dithering for camera collisions
    if (camera_cast1->is_colliding() && camera_cast2->is_colliding()) {
        if (colliding && camera_cast1->get_collider() != colliding) {
            colliding->set_visible(true);
            colliding = Object::cast_to<Node3D>(camera_cast1->get_collider());
            colliding->set_visible(false);
        } else {
            colliding = Object::cast_to<Node3D>(camera_cast1->get_collider());
            colliding->set_visible(false);
        }
    }
    if (!camera_cast1->is_colliding() && !camera_cast2->is_colliding() && colliding) {
        colliding->set_visible(true);
        colliding = NULL;
    }
}

void Player::_physics_process(double delta) {
    
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // sets rotate mode or strafe mode
    if (Input::get_singleton()->is_action_just_pressed("R")) {
        AD_rotate = !AD_rotate;
    }
    // reset gravity 
    if (this->is_on_floor()) {
        speed = 1;
        gravity = 1400.0;
        momentum = Vector3(0, 0, 0);
    }

    // gravity and jumping
    if (!this->is_on_floor()) {
        if (!hanging) {
            translate_object_local(momentum);
            velocity.y -= gravity * delta;
            speed -= air_resistance * delta;
        }
        else {
            gravity = 0;
        }

    }
    if (Input::get_singleton()->is_action_just_pressed("Jump") && this->is_on_floor()) {
        velocity.y = jump_velocity;
        jumped = true;
    }
    if (Input::get_singleton()->is_action_just_pressed("Jump") && !this->is_on_floor() && jumped) {
        gravity = 1400.0;
        velocity.y = jump_velocity;
        jumped = false;
    }
    if (Input::get_singleton()->is_action_just_pressed("Jump") && hanging) {
        gravity = 1400.0;
        velocity.y = jump_velocity;
        jumped = true;
        hanging = false;
    }
    
    // ledge stop and ledge hang 
    if (Input::get_singleton()->is_action_pressed("Shift")) {
        if (ray1->is_colliding() && ray2->is_colliding() &&
            ray3->is_colliding() && ray4->is_colliding()) {
            // WASD movement
            if (AD_rotate) {
                momentum = rotate_wasd();
            }
            else {
                momentum = strafe_wasd();
            }
        }
    } else if (Input::get_singleton()->is_action_pressed("H")) {
        if (ray1->is_colliding() || ray2->is_colliding() ||
            ray3->is_colliding() || ray4->is_colliding()) {
            // WASD movement
            if (AD_rotate) {
                momentum = rotate_wasd();
            }
            else {
                momentum = strafe_wasd();
            }
        } else {
            gravity = 0;
            velocity.y = 0;
            hanging = true;
        }
    } else {
        // WASD movement
        if (AD_rotate) {
            momentum = rotate_wasd();
        }
        else {
            momentum = strafe_wasd();
        }

    }
    gliding();
    
    end_conditions();
    set_velocity(velocity);
    move_and_slide();
}

Vector3 Player::rotate_wasd() {
    Vector3 result = Vector3(0, 0, 0);
    if (!hanging) {
        if (Input::get_singleton()->is_action_pressed("W")) {
            if (this->is_on_floor()) {
                translate_object_local(-transform.get_basis().get_column(2) * speed);
            }
            result += -transform.get_basis().get_column(2) * speed;
        }
        if (Input::get_singleton()->is_action_pressed("S")) {
            if (this->is_on_floor()) {
                translate_object_local(transform.get_basis().get_column(2) * speed);
            }
            result += transform.get_basis().get_column(2) * speed;
        }
        if (Input::get_singleton()->is_action_pressed("A")) {
            camera->set_as_top_level(true);
            rotate_object_local(Vector3(0, 1, 0), 0.05);
            camera->set_as_top_level(false);
        }
        if (Input::get_singleton()->is_action_pressed("D")) {
            camera->set_as_top_level(true);
            rotate_object_local(Vector3(0, 1, 0), -0.05);
            camera->set_as_top_level(false);
        }
    }
    if (result == Vector3(0, 0, 0)) {
        return momentum;
    } else {
        return result;
    }
}

Vector3 Player::strafe_wasd() {
    Vector3 result = Vector3(0, 0, 0);
    if (!hanging) {
        if (Input::get_singleton()->is_action_pressed("W")) {
            if (this->is_on_floor()) {
                translate_object_local(-transform.get_basis().get_column(2) * speed);
            }
            result += -transform.get_basis().get_column(2) * speed;
        }
        if (Input::get_singleton()->is_action_pressed("S")) {
            if (this->is_on_floor()) {
                translate_object_local(transform.get_basis().get_column(2) * speed);
            }
            result += transform.get_basis().get_column(2) * speed;
        }
        if (Input::get_singleton()->is_action_pressed("A")) {
            if (this->is_on_floor()) {
                translate_object_local(-transform.get_basis().get_column(0) * speed);
            }
            result += -transform.get_basis().get_column(0) * speed;
        }
        if (Input::get_singleton()->is_action_pressed("D")) {
            if (this->is_on_floor()) {
                translate_object_local(transform.get_basis().get_column(0) * speed);
            }
            result += transform.get_basis().get_column(0) * speed;
        }
    }
    if (result == Vector3(0, 0, 0)) {
        return momentum;
    } else {
        return result;
    }
}

void Player::initialize_sound() {
    String squish_path = "res://audio/squish.mp3";
    Ref<FileAccess> squish_file = FileAccess::open(squish_path, FileAccess::ModeFlags::READ);
    FileAccess *squish_ptr = Object::cast_to<FileAccess>(*squish_file);
    interact = memnew(AudioStreamMP3);
    interact->set_data(squish_ptr->get_file_as_bytes(squish_path));
    interact_player = get_node<AudioStreamPlayer>("InteractPlayer");

    String clonk_path = "res://audio/clonk.mp3";
    Ref<FileAccess> clonk_file = FileAccess::open(clonk_path, FileAccess::ModeFlags::READ);
    FileAccess *clonk_ptr = Object::cast_to<FileAccess>(*clonk_file);
    empty_interact = memnew(AudioStreamMP3);
    empty_interact->set_data(clonk_ptr->get_file_as_bytes(clonk_path));
    empty_interact_player = get_node<AudioStreamPlayer>("EmptyInteractPlayer");
}

void Player::play_interact() {
    if (interact_player && !Engine::get_singleton()->is_editor_hint()) {
        interact_player->set_stream(interact);
        interact_player->set_volume_db(-12.0);
        interact_player->play(0.0);
    }
}

void Player::play_empty_interact() {
    if (empty_interact_player && !Engine::get_singleton()->is_editor_hint()) {
        empty_interact_player->set_stream(empty_interact);
        empty_interact_player->set_volume_db(-17.0);
        empty_interact_player->play(0.0);
    }
}

void Player::food_interaction(bool entered) {
    if (entered && Input::get_singleton()->is_action_just_pressed("E")) {
        if (!interact_player->is_playing() && !mute_sound_effects) {
                play_interact();
        }
        lives++;
        if (food1->is_entered()) {
            food1->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 20), 
            rand.randf_range(-50, 50)));
            emit_signal("interact_orange");
        } 
        if (food2->is_entered()) {
            food2->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 20), 
            rand.randf_range(-50, 50)));
            emit_signal("interact_orange");
        } 
        if (food3->is_entered()) {
            food3->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 20), 
            rand.randf_range(-50, 50)));
            emit_signal("interact_orange");
        }
        if (food4->is_entered()) {
            food4->set_position(Vector3(rand.randf_range(-50, 50), rand.randf_range(2, 20), 
            rand.randf_range(-50, 50)));
            emit_signal("interact_orange");
        } 
    }
}

void Player::ledge_hang() {
     // ledge stop and ledge hang 
    if (Input::get_singleton()->is_action_pressed("Shift")) {
        if (ray1->is_colliding() && ray2->is_colliding() &&
            ray3->is_colliding() && ray4->is_colliding()) {
            // WASD movement
            if (AD_rotate) {
                momentum = rotate_wasd();
            }
            else {
                momentum = strafe_wasd();
            }
        }
    } else if (Input::get_singleton()->is_action_pressed("H")) {
        if (ray1->is_colliding() || ray2->is_colliding() ||
            ray3->is_colliding() || ray4->is_colliding()) {
            // WASD movement
            if (AD_rotate) {
                momentum = rotate_wasd();
            }
            else {
                momentum = strafe_wasd();
            }
        } else {
            gravity = 0;
            velocity.y = 0;
            hanging = true;
        }
    } else {
        // WASD movement
        if (AD_rotate) {
            momentum = rotate_wasd();
        }
        else {
            momentum = strafe_wasd();
        }
    }
}

void Player::gliding() {
       // gliding (g)
    if (Input::get_singleton()->is_action_pressed("G") && velocity.y < 0) {
        gravity = glide_gravity;
        current_air = air_resistance;
        air_resistance = 0;
    }
    if (Input::get_singleton()->is_action_just_released("G")) {
        gravity = 1400.0;
        air_resistance = current_air;
    }
}

void Player::set_gravity(float p_gravity) {
    gravity = p_gravity;
}

float Player::get_gravity() {
    return gravity;
}

void Player::set_slide_angle(float p_angle) {
    set_floor_max_angle(p_angle);
}

float Player::get_slide_angle() {
    return get_floor_max_angle();
}

void Player::set_jump_force(float p_force) {
    jump_velocity = p_force;
}

float Player::get_jump_force() {
    return jump_velocity;
}

void Player::set_glide_gravity(float p_glide_gravity) {
    glide_gravity = p_glide_gravity;
}

float Player::get_glide_gravity() {
    return glide_gravity;
}

void Player::set_air_resistance(float p_air_resistance) {
    air_resistance = p_air_resistance;
}

float Player::get_air_resistance() {
    return air_resistance;
}

bool Player::get_ad_rotate() {
    return AD_rotate;
}

int Player::get_lives() {
    return lives;
}

bool Player::get_sound_toggle() {
    return mute_sound_effects;
}

void Player::set_lives(int p_lives) {
    lives = p_lives;
}

void Player::life_lost_GUI() {
    emit_signal("life_lost_attacker");
}

void Player::toggles() {
    if (mute_sound_effects) {
        emit_signal("sound_effect_toggle", "(muted)");
    }
    if (!mute_sound_effects) {
        emit_signal("sound_effect_toggle", "(unmuted)");
    }
    if (AD_rotate) {
        emit_signal("rotate_mode_toggle", "(AD Keys)");
    }
    if (!AD_rotate) {
        emit_signal("rotate_mode_toggle", "(Mouse Movement)");
    }
}

void Player::end_conditions() {
    if (get_position().y < -100.0) {
        tree->change_scene_to_file("res://scenes/off_map.tscn");
    }
    else if (lives < 0) {
        tree->change_scene_to_file("res://scenes/no_lives.tscn");

    }
    if (lives == 10) {
        tree->change_scene_to_file("res://scenes/win_screen.tscn");
    }
}