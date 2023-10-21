#ifndef EATER_H
#define EATER_H

#include <godot_cpp/classes/a_star3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include "raycast.h"
#include "food.h"
#include "camera.h"
#include "player.h"


using namespace godot;

class Eater : public CharacterBody3D {
    GDCLASS(Eater, CharacterBody3D)

    private:
        /* data */
        Vector3 position;
        Vector3 velocity;
        Vector3 momentum;
        Input *input;
        Transform3D transform;
        Player *player;

        Food *food1;
        Food *food2;
        Food *food3;
        Food *food4;
        AStar3D *a_star;
        RandomNumberGenerator rand;

        // sounds
        AudioStreamPlayer *interact_player;
        AudioStreamMP3 *interact;
        AudioStreamMP3 *empty_interact;

        // movement
        double gravity;
        double glide_gravity;
        double jump_velocity;
        double speed;
        double air_resistance;
        double current_air;
        bool jumped;

    protected:
        static void _bind_methods();

    public:
        Eater();
        ~Eater();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;

        void initialize_sound();
        void play_interact();

        void set_gravity(float p_gravity);
        float get_gravity();

        void set_slide_angle(float p_angle);
        float get_slide_angle();

        void set_jump_force(float p_force);
        float get_jump_force();

        void set_glide_gravity(float p_glide_gravity);
        float get_glide_gravity();

        void set_air_resistance(float p_air_resistance);
        float get_air_resistance();

        bool get_sound_toggle();
        void toggles();
        void food_interaction(bool entered);
};
    

#endif

