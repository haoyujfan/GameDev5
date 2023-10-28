#ifndef ATTACKER_H
#define ATTACKER_H

#include <godot_cpp/classes/a_star3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include "raycast.h"
#include "player.h"
#include "camera.h"

using namespace godot;

class Attacker : public CharacterBody3D {
    GDCLASS(Attacker, CharacterBody3D)

    private:
        /* data */
        Vector3 position;
        Vector3 velocity;
        Player *player;
        AStar3D *a_star;
        RandomNumberGenerator rand;


        // movement
        double gravity;
        double glide_gravity;
        double jump_velocity;
        double speed;
        double air_resistance;
        double current_air;

    protected:
        static void _bind_methods();

    public:
        Attacker();
        ~Attacker();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;

        void set_gravity(float p_gravity);
        float get_gravity();

        void set_slide_angle(float p_angle);
        float get_slide_angle();

        void set_jump_force(float p_force);
        float get_jump_force();

        void set_air_resistance(float p_air_resistance);
        float get_air_resistance();
};
    

#endif

