#include "navigation.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Navigation::_bind_methods() {}

Navigation::Navigation() {}

Navigation::~Navigation() {}

void Navigation::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    astar = memnew(AStar3D);
}

void Navigation::chase(Node3D *source, Node3D *target) {}

void Navigation::retreat(Node3D *source, Node3D *target) {}

void Navigation::teleport(Node3D *target, Vector3 dest) {
    target->set_position(Vector3(0, 10, 0));
}