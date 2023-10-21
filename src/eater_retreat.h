#ifndef EATERRETREAT_H
#define EATERRETREAT_H

#include "state.h"
#include "eater.h"
#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class EaterRetreat : public State {
    // GDCLASS NOT WORKING BUT NEED IT TO GET IT IN THE GUI
    GDCLASS(EaterRetreat, State)

    private:
        Eater *eater;

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
