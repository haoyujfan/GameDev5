#ifndef EATERCHASE_H
#define EATERCHASE_H

#include "state.h"
#include "eater.h"
#include "food.h"
#include "player.h"
#include "raycast.h"
#include "navigation.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class EaterChase : public State {
    GDCLASS(EaterChase, State)

    private:
        Eater *eater;
        AStar3D *a_star;
        Food *food1;
        Food *food2;
        Food *food3;
        Food *food4;
        Player *player;
        Raycast *raycast1;
        Raycast *raycast2;
        Raycast *raycast3;
        Raycast *raycast4;
        Raycast *foodcast;
        Navigation *nav;

    protected:

    public:
        EaterChase();
        ~EaterChase();

        void _ready() override;

        void enter();
        void exit();
        void update(double delta);
        void physics_update(double delta);
};
    
}

#endif
