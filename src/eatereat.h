#ifndef EATEREAT_H
#define EATEREAT_H

#include "eaterstate.h"

class Eatereat : public Eaterstate {

    private:

    protected:

    public:
        void Enter();
        void Exit();
        void Update(double delta);
        void Physics_Update(double delta);
};
    

#endif
