#ifndef EATERRETREAT_H
#define EATERRETREAT_H

#include "state.h"
#include "eater.h"
#include "player.h"
#include "raycast.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class EaterRetreat : public State {
    GDCLASS(EaterRetreat, State)

    private:
        Eater *eater;
        Player *player;
        Raycast *raycast1;
        Raycast *raycast2;
        Raycast *raycast3;
        Raycast *raycast4;

    protected:

    public:
        EaterRetreat();
        ~EaterRetreat();

        void _ready() override;

        void enter();
        void exit();
        void update(double delta);
        void physics_update(double delta);
};
    
}

#endif
