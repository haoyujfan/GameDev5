#include "eatereat.h"
#include <godot_cpp/variant/utility_functions.hpp>

#include <cstdlib>

void Eatereat::Enter() {
    UtilityFunctions::print("enter eat state");
}

void Eatereat::Update(double delta) {

}

void Eatereat::Physics_Update(double delta) {

}

void Eatereat::Exit() {
    UtilityFunctions::print("exit eat state");
}