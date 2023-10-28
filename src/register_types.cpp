#include "register_types.h"
#include "player.h"
#include "cactus.h"
#include "food.h"
#include "ground.h"
#include "wall.h"
#include "camera.h"
#include "raycast.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_gdextension_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ClassDB::register_class<Player>();
    ClassDB::register_class<Cactus>();
    ClassDB::register_class<Food>();
    ClassDB::register_class<Ground>();
    ClassDB::register_class<Wall>();
    ClassDB::register_class<Camera>();
    ClassDB::register_class<Raycast>();
}

void uninitialize_gdextension_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gdextension_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_gdextension_module);
    init_obj.register_terminator(uninitialize_gdextension_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
