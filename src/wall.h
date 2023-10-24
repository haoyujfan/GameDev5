#ifndef WALL_H
#define WALL_H

#include <godot_cpp/classes/static_body3d.hpp>

namespace godot {

class Wall : public StaticBody3D {
    GDCLASS(Wall, StaticBody3D)

private:

protected:
    static void _bind_methods();

public:
    Wall();
    ~Wall();

    void _ready() override;
    void _process(double delta) override;
};

}

#endif