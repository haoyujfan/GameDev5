#include "player.h"
#include <godot_cpp/classes/csg_mesh3d.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
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
#include <godot_cpp/classes/multiplayer_synchronizer.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

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
    ClassDB::bind_method(D_METHOD("set_lives", "lives"), &Player::set_lives);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "lives", PROPERTY_HINT_RANGE, 
        "0, 2, 0.1"), "set_lives", "get_lives");

    ClassDB::bind_method(D_METHOD("play_hurt"), &Player::play_hurt);

    ClassDB::bind_method(D_METHOD("move_food", "food", "pos"), &Player::move_food);
    ClassDB::bind_method(D_METHOD("broadcast_food", "food", "pos"), &Player::broadcast_food);

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
    //position = Vector3(0.0, 10.0, 0.0);
    hanging = false;
    AD_rotate = true;
    mute_sound_effects = false;
    hurt_frames = 0;
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
    ray_up1 = get_node<Raycast>("RaycastUp");
    ray_up2 = get_node<Raycast>("RaycastUp2");
    ray_up3 = get_node<Raycast>("RaycastUp3");
    ray_up4 = get_node<Raycast>("RaycastUp4");
    camera_cast1 = get_node<Raycast>("Node3D/Camera/Raycast");
    camera_cast2 = get_node<Raycast>("Node3D/Camera/Raycast2");
    colliding = NULL;

    food1 = get_node<Food>("../Food");
    food2 = get_node<Food>("../Food2");
    food3 = get_node<Food>("../Food3");
    food4 = get_node<Food>("../Food4");

    camera = get_node<Camera>("Node3D/Camera");
    
    Ref<Material> mat_ref = get_node<CSGMesh3D>("CSGMesh3D")->get_material();
    //FileAccess *squish_ptr = Object::cast_to<FileAccess>(*squish_file);
    material = Object::cast_to<BaseMaterial3D>(*mat_ref);
    albedo = material->get_albedo();
    tree = get_tree();

    sync = get_node<MultiplayerSynchronizer>("MultiplayerSynchronizer");

    if (sync->get_multiplayer_authority() == get_multiplayer()->get_unique_id()) {
        camera->make_current();
    }

    Dictionary *rpc_annotations_1 = new Dictionary();
    (*rpc_annotations_1)["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
    (*rpc_annotations_1)["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
    (*rpc_annotations_1)["call_local"] = true;
    (*rpc_annotations_1)["channel"] = 0;
    rpc_config("move_food", *rpc_annotations_1);

    Dictionary *rpc_annotations_2 = new Dictionary();
    (*rpc_annotations_2)["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
    (*rpc_annotations_2)["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
    (*rpc_annotations_2)["call_local"] = false;
    (*rpc_annotations_2)["channel"] = 0;
    rpc_config("broadcast_food", *rpc_annotations_2);
}

void Player::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (sync->get_multiplayer_authority() == get_multiplayer()->get_unique_id()) {
        // handle food interactions
        bool entered_by_player = food1->is_entered_by_player() || food2->is_entered_by_player() || 
            food3->is_entered_by_player() || food4->is_entered_by_player();
        if (food1->get_enter_class() == "Player" || food2->get_enter_class() == "Player" ||
            food3->get_enter_class() == "Player" || food4->get_enter_class() == "Player") {
            food_interaction(entered_by_player);
        }

        // empty interaction
        if (!entered_by_player && Input::get_singleton()->is_action_just_pressed("E")) {
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
}

void Player::_physics_process(double delta) {
    
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (sync->get_multiplayer_authority() == get_multiplayer()->get_unique_id()) {
    // if (sync->get_multiplayer_authority() == 1) {
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

        // getting jumped on
        if ((ray_up1->is_colliding() && ray_up1->get_collider()->get_class() == "Player") ||
            (ray_up2->is_colliding() && ray_up2->get_collider()->get_class() == "Player") ||
            (ray_up3->is_colliding() && ray_up3->get_collider()->get_class() == "Player")||
            (ray_up4->is_colliding() && ray_up4->get_collider()->get_class() == "Player")) {
                attack();
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
        if (is_hurt) {
            material->set_albedo(Color(.5, 0, 0, .5));
            hurt_frames++;
            if (hurt_frames > 20) {
                material->set_albedo(albedo);
                is_hurt = false;
                hurt_frames = 0;
            }
        }
        gliding();
        
        end_conditions();
        set_velocity(velocity);
        move_and_slide();
    }
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

    String hurt_path = "res://audio/hurt.mp3";
    Ref<FileAccess> hurt_file = FileAccess::open(hurt_path, FileAccess::ModeFlags::READ);
    FileAccess *hurt_ptr = Object::cast_to<FileAccess>(*hurt_file);
    hurt = memnew(AudioStreamMP3);
    hurt->set_data(hurt_ptr->get_file_as_bytes(hurt_path));
    hurt_player = get_node<AudioStreamPlayer>("HurtPlayer");
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

void Player::play_hurt() {
    if (hurt_player && !Engine::get_singleton()->is_editor_hint() && !mute_sound_effects) {
        hurt_player->set_stream(hurt);
        hurt_player->set_volume_db(-12.0);
        hurt_player->play(0.0);
    }
}

void Player::food_interaction(bool entered_by_player) {
    if (entered_by_player && Input::get_singleton()->is_action_just_pressed("E")) {
        emit_signal("interact_orange");
        if (!interact_player->is_playing() && !mute_sound_effects) {
            play_interact();
        }
        lives++;
        if (food1->is_entered_by_player()) {
            Vector3 pos1 = Vector3(rand.randf_range(-150, 150), rand.randf_range(4, 20), 
            rand.randf_range(-150, 150));
            rpc_id(1, "move_food", "", pos1);
            food1->entered_by_player = false;
        } 
        if (food2->is_entered_by_player()) {
            Vector3 pos2 = Vector3(rand.randf_range(-150, 150), rand.randf_range(4, 20), 
            rand.randf_range(-150, 150));
            rpc_id(1, "move_food", "2", pos2);
            food2->entered_by_player = false;
        } 
        if (food3->is_entered_by_player()) {
            Vector3 pos3 = Vector3(rand.randf_range(-150, 150), rand.randf_range(4, 20), 
            rand.randf_range(-150, 150));
            rpc_id(1, "move_food", "3", pos3);
            food3->entered_by_player = false;
        }
        if (food4->is_entered_by_player()) {
            Vector3 pos4 = Vector3(rand.randf_range(-150, 150), rand.randf_range(4, 20), 
            rand.randf_range(-150, 150));
            rpc_id(1, "move_food", "4", pos4);
            food4->entered_by_player = false;
        } 
    }
}

void Player::move_food(String food, Vector3 pos) {
    get_node<Food>("../Food" + food)->set_position(pos);
    rpc("broadcast_food", food, pos);
}

void Player::broadcast_food(String food, Vector3 pos) {
    get_node<Food>("../Food" + food)->set_position(pos);
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

void Player::attack() {
    Object *collider;
    if (ray_up1->is_colliding() && ray_up1->get_collider()->get_class() == "Player") {
        collider = ray_up1->get_collider();
    }
    if (ray_up2->is_colliding() && ray_up2->get_collider()->get_class() == "Player") {
        collider = ray_up2->get_collider();
    }
    if (ray_up3->is_colliding() && ray_up3->get_collider()->get_class() == "Player") {
        collider = ray_up3->get_collider();
    }
    if (ray_up4->is_colliding() && ray_up4->get_collider()->get_class() == "Player") {
        collider = ray_up4->get_collider();
    }
    Node3D *collider_node = Object::cast_to<Node3D>(collider);
    Vector3 collider_pos = collider_node->get_position();
    collider_pos.y += 20;
    //collider_node->set_position(collider_pos);
    collider_node->set_position(Vector3(0, 100, 0));       
    set_position(Vector3(rand.randf_range(-100, 100), 5, 0));
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

void Player::set_lives(int p_lives) {
    lives = p_lives;
}

bool Player::get_sound_toggle() {
    return mute_sound_effects;
}



void Player::life_lost_GUI() {
    emit_signal("life_lost_attacker");
}

void Player::set_is_hurt(bool p_is_hurt) {
    is_hurt = p_is_hurt;
}

void Player::set_hurt_frames(int frames) {
    hurt_frames = frames;
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