#ifndef FOOD_H
#define FOOD_H

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>

namespace godot {

class Food : public Area3D {
    GDCLASS(Food, Area3D)

private:
    RandomNumberGenerator rand;
    Vector3 position;
    int value;
    bool entered;
    String enter_class;

protected:
    static void _bind_methods();

public:
    Food();
    ~Food();
    void _process(double delta) override;
    void _ready() override;

    void food_body_entered(const Node3D *node);
    void food_body_exited(const Node3D *node);
    bool is_entered();
    bool is_entered_by_player();
    String get_enter_class();

    bool entered_by_player;

};

}

#endif