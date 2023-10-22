#ifndef EATEREAT_H
#define EATEREAT_H

#include "state.h"
#include "eater.h"
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>



namespace godot {

class EaterEat : public State {
    // GDCLASS NOT WORKING BUT NEED IT TO GET IT IN THE GUI
    GDCLASS(EaterEat, State)

    private:
        Eater *eater;
        RandomNumberGenerator rand;
        Food *curr_food;

    protected:
        static void _bind_methods();

    public:
        EaterEat();
        ~EaterEat();

        void _ready() override;

        void enter() override;
        void exit() override;
        void update(double delta);
        void physics_update(double delta);
        void eat_food(String food_name);
};
    
}

#endif
