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
    UtilityFunctions::print("in navigation");
}

void Navigation::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }

}