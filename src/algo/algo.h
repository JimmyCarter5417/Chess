#ifndef ALGO_H
#define ALGO_H

#include "util/def.h"
#include "model/model.h"

namespace algo
{
    int minimax(int depth, model::IModel* model, def::EPlayer maxPlayer, def::TMove& move);
    int alphabeta(int depth, model::IModel* model, def::EPlayer maxPlayer, def::TMove& move, int alpha, int beta);
}

#endif // ALGO_H
