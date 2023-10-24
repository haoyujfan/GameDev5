#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/a_star3d.hpp>

namespace godot {

class Navigation : public AStar3D {
    GDCLASS(Navigation, AStar3D)

private:

protected:
    static void _bind_methods();

public:
    Navigation();
    ~Navigation();
    void _process(double delta);
    void _ready();

};

}

#endif