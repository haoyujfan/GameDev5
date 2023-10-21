#ifndef STATE_H
#define STATE_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

class State : public Node {
    GDCLASS(State, Node)

private:

protected:
    static void _bind_methods();

public:
    State();
    ~State();
    void _ready() override;
    virtual void enter();
    virtual void exit();
    virtual void update(double delta);
    virtual void physics_update(double delta);
};

}

#endif