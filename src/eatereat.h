#ifndef EATEREAT_H
#define EATEREAT_H

#include "eaterstate.h"
#include <godot_cpp/classes/character_body3d.hpp>

class Eatereat : public Eaterstate {

    private:
        CharacterBody3D *eater;

    protected:

    public:
        Eatereat();
        ~Eatereat();

        void _ready() override;

        void Enter();
        void Exit();
        void Update(double delta);
        void Physics_Update(double delta);
};
    

#endif
