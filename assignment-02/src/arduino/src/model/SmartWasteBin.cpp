#include "SmartWasteBin.h"

SmartWasteBin::SmartWasteBin() {
}

void SmartWasteBin::init() {
    
    state = IDLE;
}

void SmartWasteBin::setState(State s) {
    state = s;
}
