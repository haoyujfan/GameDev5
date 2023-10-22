#ifndef ATTACKERCHASE_H
#define ATTACKERCHASE_H

#include "state.h"
#include "attacker.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class AttackerChase : public State {
    GDCLASS(AttackerChase, State)

    private:
        Attacker *attacker;
        AStar3D *a_star;

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
