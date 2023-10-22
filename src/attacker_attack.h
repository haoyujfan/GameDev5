#ifndef ATTACKERATTACK_H
#define ATTACKERATTACK_H

#include "state.h"
#include "attacker.h"
#include "player.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class AttackerAttack : public State {
    GDCLASS(AttackerAttack, State)

    private:
        Attacker *attacker;
        AStar3D *a_star;
        Player *player;

    protected:

    public:
        AttackerAttack();
        ~AttackerAttack();

        void _ready() override;

        void enter();
        void exit();
        void update(double delta);
        void physics_update(double delta);
};
    
}

#endif
