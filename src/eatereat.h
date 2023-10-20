#ifndef EATEREAT_H
#define EATEREAT_H

#include "state.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class EaterEat : public State {

    private:
        CharacterBody3D *eater;

    protected:

    public:
        EaterEat();
        ~EaterEat();

        void _ready() override;

        void Enter();
        void Exit();
        void Update(double delta);
        void Physics_Update(double delta);
};
    
}

#endif
