#ifndef CAMERA_H
#define CAMERA_H

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

namespace godot {

class Camera : public Camera3D {
    GDCLASS(Camera, Camera3D)

private:
    Vector3 rotation;
    float speed;

protected:
    static void _bind_methods();

public:
    Camera();
    ~Camera();
    void _input(const Ref<InputEvent> &event);
    void set_speed(float s);
    float get_speed();

};

}

#endif