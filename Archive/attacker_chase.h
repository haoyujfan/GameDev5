#ifndef ATTACKERCHASE_H
#define ATTACKERCHASE_H

#include "state.h"
#include "attacker.h"
#include "player.h"
#include "raycast.h"
#include "navigation.h"

#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class AttackerChase : public State {
    GDCLASS(AttackerChase, State)

    private:
        Attacker *attacker;
        AStar3D *a_star;
        Player *player;
        Raycast *raycast1;
        Raycast *raycast2;
        Raycast *raycast3;
        Raycast *raycast4;
        Navigation *nav;

    protected:

    public:
        AttackerChase();
        ~AttackerChase();

        void _ready() override;

        void enter();
        void exit();
        void update(double delta);
        void physics_update(double delta);
};
    
}

#endif
