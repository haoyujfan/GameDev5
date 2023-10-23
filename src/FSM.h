#ifndef FSM_H
#define FSM_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "eater.h"
#include "attacker.h"
#include "state.h"

using namespace godot;

class FSM : public Node {
    GDCLASS(FSM, Node)

    private:
        Dictionary states;
        State *current_state;
        State *initial_state;
        TypedArray<Node> children;
        Node *parent;

    protected:
        static void _bind_methods();

    public:
        FSM();
        ~FSM();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
        void on_child_transition(String old_state_name, String new_state_name);
};
    

#endif

