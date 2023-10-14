#ifndef PLAYER_H
#define PLAYER_H

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

using namespace godot;

class Player : public CharacterBody3D {
    GDCLASS(Player, CharacterBody3D)

    private:
        /* data */
        Vector3 position;
        Vector3 velocity;
        Vector3 momentum;
        Input *input;
        Transform3D transform;
        Raycast *ray1;
        Raycast *ray2;
        Raycast *ray3;
        Raycast *ray4;
        SceneTree *tree;
        int lives;

        Food *food1;
        Food *food2;
        Food *food3;
        Food *food4;
        RandomNumberGenerator rand;

        Camera *camera;
        Raycast *camera_cast1;
        Raycast *camera_cast2;
        Node3D *colliding;

        // sounds
        AudioStreamPlayer *interact_player;
        AudioStreamPlayer *empty_interact_player;
        AudioStreamMP3 *interact;
        AudioStreamMP3 *empty_interact;
        bool mute_sound_effects;

        // movement
        double gravity;
        double glide_gravity;
        double jump_velocity;
        double speed;
        double air_resistance;
        double current_air;
        bool jumped;
        bool hanging;
        bool AD_rotate;

    protected:
        static void _bind_methods();

    public:
        Player();
        ~Player();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
        Vector3 rotate_wasd();
        Vector3 strafe_wasd();

        void initialize_sound();
        void play_empty_interact();
        void play_interact();
        bool get_ad_rotate();

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

        void ledge_hang();
        void gliding();
           
        bool get_sound_toggle();
        int get_lives();
        void set_lives(int p_lives);
        void toggles();
        void end_conditions();
        void food_interaction(bool entered);
};
    

#endif

