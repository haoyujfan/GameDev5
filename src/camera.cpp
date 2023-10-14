#include "camera.h"
#include "player.h"
#include <godot_cpp/core/class_db.hpp>
#include <cstdlib>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Camera::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_speed"), &Camera::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Camera::set_speed);
    ClassDB::add_property("Camera", PropertyInfo(Variant::FLOAT, "camera sensitivity", PROPERTY_HINT_RANGE, 
        "0.0, 900.0, 1.0"), "set_speed", "get_speed");
}

Camera::Camera() {
    rotation = Vector3(0.0, 0.0, 0.0);
    speed = 0;
}

Camera::~Camera() {}

void Camera::_input(const Ref<InputEvent> &event) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
	const InputEventMouseMotion *key_event = Object::cast_to<const InputEventMouseMotion>(*event);
    
	if (key_event) {
        Node3D* tgt = Object::cast_to<Node3D>(get_parent());
        Player* player = get_node<Player>("../../../Player");
        if (!player->get_ad_rotate()) {
            rotation[0] = 0.0;
            rotation += Vector3(0.0, -1 * key_event->get_relative()[0] / (1000 - speed), 0.0);
            player->set_rotation(rotation);
        } else {
            rotation += Vector3(0.0, -1 * key_event->get_relative()[0] / (1000 - speed), 0.0);     
            tgt->set_rotation(rotation);
        }
	}
}

void Camera::set_speed(float s) {
    speed = s;
}

float Camera::get_speed() {
    return speed;
}


