#ifndef ATTACKERDODGE_H
#define ATTACKERDODGE_H

#include "state.h"
#include "attacker.h"
#include "player.h"
#include "raycast.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class AttackerDodge : public State {
    GDCLASS(AttackerDodge, State)

    private:
        Attacker *attacker;
        AStar3D *a_star;
        Player *player;
        Raycast *raycast1;
        Raycast *raycast2;
        Raycast *raycast3;
        Raycast *raycast4;

    protected:

    public:
        AttackerDodge();
        ~AttackerDodge();

        void _ready() override;

        void enter();
        void exit();
        void update(double delta);
        void physics_update(double delta);
};
    
}

#endif
