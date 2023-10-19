#ifndef EATERSTATE_H
#define EATERSTATE_H

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class Eaterstate : public Node {
    GDCLASS(Eaterstate, Node)

    private:
        

    protected:
        static void _bind_methods();

    public:
        Eaterstate();
        ~Eaterstate();

        // void _process(double delta) override;
        // void _physics_process(double delta) override;
        // void _ready() override;

        void Enter();
        void Exit();
        void Update(double delta);
        void Physics_Update(double delta);
};
    

#endif
