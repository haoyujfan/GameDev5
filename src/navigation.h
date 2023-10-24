#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/a_star3d.hpp>

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

    void chase(Node3D *source, Node3D *target);
    void retreat(Node3D *source, Node3D *target);
    void teleport(Node3D *target, Vector3 dest);
};

}

#endif