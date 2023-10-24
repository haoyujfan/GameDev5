#include "wall.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Wall::_bind_methods() {}

// constructor
Wall::Wall() {}

// destructor
Wall::~Wall() {}

void Wall::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void Wall::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}
