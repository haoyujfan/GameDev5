#include "cactus.h"
#include "ground.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <stdlib.h>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

// bind c++ methods to godot
void Cactus::_bind_methods() {
    ClassDB::bind_method(D_METHOD("cactus_body_entered", "area"), &Cactus::cactus_body_entered);

    ADD_SIGNAL(MethodInfo("interact_cactus"));
}

// constructor
Cactus::Cactus() {

}

// destructor
Cactus::~Cactus() {

}

// update the new position based on speed and trajectory
void Cactus::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (Input::get_singleton()->is_action_just_pressed("Sound Effect")) {
        mute_sound_effects = !mute_sound_effects;
    }
}

// initialize the cactus when its children are ready 
void Cactus::_ready() {
    initialize_sound();
    this->connect("body_entered", Callable(this, "cactus_body_entered"));
    mute_sound_effects = false;
    player = get_node<Player>("../Player");
}

void Cactus::initialize_sound() {
    String hurt_path = "res://audio/hurt.mp3";
    Ref<FileAccess> hurt_file = FileAccess::open(hurt_path, FileAccess::ModeFlags::READ);
    FileAccess *hurt_ptr = Object::cast_to<FileAccess>(*hurt_file);
    hurt = memnew(AudioStreamMP3);
    hurt->set_data(hurt_ptr->get_file_as_bytes(hurt_path));
    sound_effects = get_node<AudioStreamPlayer>("AudioStreamPlayer");
}

void Cactus::play_interact() {
    if (sound_effects && !Engine::get_singleton()->is_editor_hint() && !mute_sound_effects) {
        sound_effects->set_stream(hurt);
        sound_effects->set_volume_db(-12.0);
        sound_effects->play(0.0);
    }
}

// handle collisions with other objects
void Cactus::cactus_body_entered(const Node3D* node) {
    if (node->get_class() == "Player") {
        play_interact();
        emit_signal("interact_cactus");
        int lives = player->get_lives();
        player->set_lives(lives - 1);
    }
}

