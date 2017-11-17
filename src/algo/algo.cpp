#include "algo.h"

#include <algorithm>
#include <limits.h>

int algo::minimax(int depth, model::IModel* model, def::EPlayer maxPlayer, def::TMove& move)
{
    // if (depth >= 7 || model == nullptr)
    //     return 0;

    int score = model->getScore(maxPlayer);
    if (depth == 0 || score == INT_MAX || score == INT_MIN)
        return score;

    int movesCount = model->generateAllMoves();
    if (movesCount <= 0)
        return 0;

    int maxRes = INT_MIN;
    int minRes = INT_MAX;
    for (int i = 0; i < movesCount; i++)
    {
        if (model->makeMove(i) & model::EMR_ok)
        {
            def::TMove nextMove;
            int score = minimax(depth - 1, model, maxPlayer, nextMove);
            model->undoMove();

            if (model->getNextPlayer() == maxPlayer)
            {
                if (score > maxRes)
                {
                    move = model->getMove(i);
                    maxRes = score;
                }
            }
            else
            {
                if (score < minRes)
                {
                    move = model->getMove(i);
                    minRes = score;
                }
            }
        }
    }

    return model->getNextPlayer() == maxPlayer ? maxRes : minRes;
}

int algo::alphabeta(int depth, model::IModel* model, def::EPlayer maxPlayer, def::TMove& move, int alpha, int beta)
{
    // if (depth >= 7 || model == nullptr)
    //     return 0;

    int score = model->getScore(maxPlayer);
    if (depth == 0 || score == INT_MAX || score == INT_MIN)
        return score;

    int movesCount = model->generateAllMoves();
    if (movesCount <= 0)
        return 0;

    for (int i = 0; i < movesCount; i++)
    {
        if (model->makeMove(i) & model::EMR_ok)
        {
            def::TMove nextMove;
            int score = alphabeta(depth - 1, model, maxPlayer, nextMove, alpha, beta);
            model->undoMove();

            move = model->getMove(i);

            if (model->getNextPlayer() == maxPlayer)
                alpha = std::max(alpha, score);
            else
                beta = std::max(beta, score);

            if (beta <= alpha)
                break;
        }
    }

    return model->getNextPlayer() == maxPlayer ? alpha : beta;
}
