#ifndef ATTACKERFSM_H
#define ATTACKERFSM_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "state.h"
#include "attacker.h"

using namespace godot;

class AttackerFSM : public Node {
    GDCLASS(AttackerFSM, Node)

    private:
        Dictionary states;
        State *current_state;
        State *initial_state;
        TypedArray<Node> children;

    protected:
        static void _bind_methods();

    public:
        AttackerFSM();
        ~AttackerFSM();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
        void on_child_transition(String old_state_name, String new_state_name);
};
    

#endif

