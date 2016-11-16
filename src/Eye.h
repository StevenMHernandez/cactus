#include "Transition.h"

#define SHOW_FRAME 1
#define SHOW_TRANSITION 2
#define SHOW_MESSAGE 3

class Eye {
public:
    int status = SHOW_TRANSITION;
    int next_status = SHOW_MESSAGE;
    Transition *transition = new Transition;

    void transitionToNextStatus() {
        if (this->status == SHOW_FRAME) {
            this->status = SHOW_TRANSITION;
            this->next_status = SHOW_MESSAGE;
        } else {
            this->status = SHOW_TRANSITION;
            this->next_status = SHOW_FRAME;
        }
    }

    void updateStatus() {
        this->status = this->next_status;
        this->next_status = SHOW_TRANSITION;
    }
};
