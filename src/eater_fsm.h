#ifndef EATERFSM_H
#define EATERFSM_H

#include <godot_cpp/classes/node.hpp>
#include "state.h"
#include "eater.h"
#include "eater_eat.h"
#include "eater_chase.h"
#include "eater_retreat.h"

using namespace godot;

class EaterFSM : public Node {
    GDCLASS(EaterFSM, Node)

    private:
        Dictionary states;
        State *current_state;
        State *initial_state;
        TypedArray<Node> children;

    protected:
        static void _bind_methods();

    public:
        EaterFSM();
        ~EaterFSM();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
        void on_child_transition(State *old_state, String new_state_name);
};
    

#endif

