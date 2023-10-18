#ifndef EATERFSM_H
#define EATERFSM_H

#include <godot_cpp/classes/node3d.hpp>
#include "eaterstate.h"
#include "eater.h"

using namespace godot;

class Eaterfsm : public Node {
    GDCLASS(Eaterfsm, Node)

    private:
        Dictionary states;
        Eaterstate current_state;
        TypedArray<Node3D> children;

    protected:
        static void _bind_methods();

    public:
        Eaterfsm();
        ~Eaterfsm();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
};
    

#endif

