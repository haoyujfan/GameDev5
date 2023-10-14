#ifndef CACTUS_H
#define CACTUS_H

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include "player.h"

namespace godot {

class Cactus : public Area3D {
    GDCLASS(Cactus, Area3D)

private:
    Vector3 position;
    bool pickup;
    AudioStreamPlayer *sound_effects;
    AudioStreamMP3 *hurt;
    bool mute_sound_effects;
    Player *player;

protected:
    static void _bind_methods();

public:
    Cactus();
    ~Cactus();
    void _process(double delta) override;
    void _ready() override;
    
    void initialize_sound();
    void play_interact();
    void cactus_body_entered(const Node3D *node);


};

}

#endif