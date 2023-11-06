#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/multiplayer_synchronizer.hpp>
#include "raycast.h"
#include "food.h"
#include "camera.h"

using namespace godot;

class Player : public CharacterBody3D {
    GDCLASS(Player, CharacterBody3D)

    private:
        /* data */
        int other_id;
        bool game_over;

        String mode;

        Vector3 position;
        Vector3 velocity;
        Vector3 momentum;
        Input *input;
        Transform3D transform;
        Raycast *ray1;
        Raycast *ray2;
        Raycast *ray3;
        Raycast *ray4;
        Array ray_up_array;

        SceneTree *tree;
        int lives;
        bool is_hurt;

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
        AudioStreamPlayer *hurt_player;
        AudioStreamMP3 *interact;
        AudioStreamMP3 *empty_interact;
        AudioStreamMP3 *hurt;
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

        // color
        BaseMaterial3D *material;
        Color albedo;
        int hurt_frames;

        // multiplayer
        MultiplayerSynchronizer *sync;

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
        void play_hurt();

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

        void set_mode(String p_mode);
        String get_mode();

        void ledge_hang();
        void gliding();
        void attack();
           
        bool get_sound_toggle();

        int get_lives();
        void set_lives(int p_lives);

        void life_lost_GUI();
        void set_is_hurt(bool p_is_hurt);
        void set_hurt_frames(int frames);

        void set_other_id(int p_id);
        int get_other_id();
       
        void set_game_over(bool p_game_over);
        bool get_game_over();


        void toggles();
        void end_conditions();
        void food_interaction(bool entered);

        void move_food(String food, Vector3 pos);
        void broadcast_food(String food, Vector3 pos);
        void win(String condition);
        void lose(String condition);

        Vector3 get_local_velocity();
        void dead_reckoning(double delta);
};
    

#endif

