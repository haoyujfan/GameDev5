#ifndef EATERCHASE_H
#define EATERCHASE_H

#include "state.h"
#include "eater.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class EaterChase : public State {
    // GDCLASS NOT WORKING BUT NEED IT TO GET IT IN THE GUI
    GDCLASS(EaterChase, State)

    private:
        Eater *eater;
        AStar3D *a_star;
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
