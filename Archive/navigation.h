#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/a_star3d.hpp>
#include "raycast.h"

namespace godot {

class Navigation : public Node3D {
    GDCLASS(Navigation, Node3D)

private:
    AStar3D *astar;

protected:
    static void _bind_methods();

public:
    Navigation();
    ~Navigation();
    void _ready() override;

    void chase(CharacterBody3D *source, Vector3 dest, double delta);
    void retreat(CharacterBody3D *source, Vector3 dest);
    void jump(CharacterBody3D *source);
    void teleport(CharacterBody3D *source, Vector3 dest);
};

}

#endif