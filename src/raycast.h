#ifndef RAYCAST_H
#define RAYCAST_H

#include <godot_cpp/classes/ray_cast3d.hpp>

namespace godot {

class Raycast : public RayCast3D {
    GDCLASS(Raycast, RayCast3D)

private:

protected:
    static void _bind_methods();

public:
    Raycast();
    ~Raycast();
    void _process(double delta) override;
    void _physics_process(double delta);
};

}

#endif