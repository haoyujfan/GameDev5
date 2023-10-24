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

void Navigation::chase(CharacterBody3D *source, Vector3 dest) {
    astar->add_point(1, source->get_position());
    astar->add_point(2, dest);
    astar->connect_points(1, 2, false);
    PackedVector3Array path = astar->get_point_path(1, 2);
    Vector3 dir = Vector3(0, 0, 0);
    if (path.size() > 0) {
        dir = path[1] - source->get_position();
    }
    dir.normalize();
    source->set_velocity(dir * 10);
    source->move_and_slide();
    source->set_position(source->get_position());
    astar->disconnect_points(1, 2);
    astar->remove_point(1);
    astar->remove_point(2);
}

void Navigation::retreat(CharacterBody3D *source, Vector3 dest) {
    astar->add_point(1, source->get_position());
    astar->add_point(2, dest);
    astar->connect_points(1, 2, false);
    PackedVector3Array path = astar->get_point_path(1, 2);
    Vector3 dir = Vector3(0, 0, 0);
    if (path.size() > 0) {
        dir = -1 * (path[1] - source->get_position());
    }
    dir.normalize();
    source->set_velocity(dir * 10);
    source->move_and_slide();
    source->set_position(source->get_position());
    astar->disconnect_points(1, 2);
    astar->remove_point(1);
    astar->remove_point(2);
}

void Navigation::teleport(CharacterBody3D *source, Vector3 dest) {
    source->set_position(dest);
}