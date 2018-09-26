
#include "State.h"

State::State(ul _gridSize) : gridSize(_gridSize),
                             W(gridSize),
                             CHO(gridSize),
                             OX(gridSize){
    for (unsigned long i = 0; i < gridSize; i++) {
        W[i] = std::vector<bool>(gridSize);
        CHO[i] = std::vector<double>(gridSize);
        OX[i] = std::vector<double>(gridSize);
    }
}
