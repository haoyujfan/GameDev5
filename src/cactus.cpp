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

    ADD_SIGNAL(MethodInfo("lose_life"));
    ADD_SIGNAL(MethodInfo("cactus_hit"));

    ADD_SIGNAL(MethodInfo("hit_player"));
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
}

// initialize the cactus when its children are ready 
void Cactus::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    player = get_node<Player>("../Player");
    this->connect("hit_player", Callable(player, "play_hurt"));
    this->connect("body_entered", Callable(this, "cactus_body_entered"));
}

// handle collisions with other objects
void Cactus::cactus_body_entered(const Node3D* node) {
    if (node->get_class() == "Player") {
        emit_signal("lose_life");
        emit_signal("hit_player");
        int lives = player->get_lives();
        player->set_lives(lives - 1);
        player->set_is_hurt(true);
        player->set_hurt_frames(0);
    }
}

