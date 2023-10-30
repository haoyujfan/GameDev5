#ifndef RAYCAST_H
#define RAYCAST_H

#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

class SceneManager : public Node3D {
    GDCLASS(SceneManager, Node3D)

private:
    PackedScene player_scene;

protected:
    static void _bind_methods();

public:
    SceneManager();
    ~SceneManager();
    void _ready() override;
    void _process(double delta) override;

};

}

#endif