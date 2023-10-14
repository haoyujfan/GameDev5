#include "ground.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Ground::_bind_methods() {
    ADD_SIGNAL(MethodInfo("music_toggle", PropertyInfo(Variant::STRING, "toggle")));
}

// constructor
Ground::Ground() {
}

// destructor
Ground::~Ground() {
}

void Ground::_ready() {
    volume = -15;
    mute_music = false;
    music_pos = 0.0;
    initialize_sound();
}

void Ground::_process(double delta) {
    music_controls();
    toggle();
}

void Ground::initialize_sound() {
    String background_path = "res://audio/background.mp3";
    Ref<FileAccess> file = FileAccess::open(background_path, FileAccess::ModeFlags::READ);
    FileAccess *background_ptr = Object::cast_to<FileAccess>(*file);
    background = memnew(AudioStreamMP3);
    background->set_data(background_ptr->get_file_as_bytes(background_path));
    background_player = get_node<AudioStreamPlayer>("../BackgroundPlayer");
    if (background_player && !Engine::get_singleton()->is_editor_hint()) {
        background_player->set_stream(background);
        background_player->set_volume_db(volume);
        background_player->play(music_pos);
    }
}

void Ground::toggle() {
    if (mute_music) {
        emit_signal("music_toggle", "(muted)");
    }
    if (!mute_music) {
        emit_signal("music_toggle", "(unmuted)");
    }
}

void Ground::music_controls() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (Input::get_singleton()->is_action_just_pressed("Music")) {
        mute_music = !mute_music;
    }
    if (mute_music && background_player->is_playing()) {
        music_pos = background_player->get_playback_position();
        background_player->stop();
    }
    if (!mute_music && !background_player->is_playing()) {
        background_player->play(music_pos);
    }
    if (Input::get_singleton()->is_action_just_pressed("Volume Up")) {
        volume += 1;
        background_player->set_volume_db(volume);
    }
    if (Input::get_singleton()->is_action_just_pressed("Volume Down")) {
        volume -= 1;
        background_player->set_volume_db(volume);
    }
}
