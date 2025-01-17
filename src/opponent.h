#ifndef OPPONENT_H
#define OPPONENT_H

#include <vector>

#include "point.h"
#include "gridstate.h"
#include "misc.h"

class Opponent
{
    public:
         enum DifficultyLevel {
            SIMPLE,
            EASY,
            MEDIUM,
            HARD
        };

    private:
        int boardSize;
        
        bool diagonalLineAttackStartTopleft = true;
        int diagonalLinePosition = 0;

        DifficultyLevel selectedLevel;
        vector<Point> moves;
        vector<State> states;
        vector<Point> freeSquares;

        void deleteFreeSquare(Point);
        bool isSquareFree(Point);
        bool isSquareHit(Point);
        bool wasLastAttackAHit();
        bool wasLastTwoAttacksHorisontal();
        bool wasLastThreeAttacksHorisontal();


        void setupFreeSquares();
        vector<Point> getAllHitsFromMoves();

        Point attackRandomFreeSquare();
        Point attackAreaAfterHit(Point lastMove);
        Point randomAttackInTheMiddle();
        Point attackAlongDiagonalLine();
        Point horisontalAttackOnShip(Point);
        Point verticalAttackOnShip(Point);
    
    public:
        Opponent(int _boardSize, DifficultyLevel _selectedLevel)
        :
        boardSize(_boardSize),
        selectedLevel(_selectedLevel)
        {
            freeSquares = vector<Point>();
            moves = vector<Point>();
            states = vector<State>();

            setupFreeSquares();
        };

        void setDiagonalAttackDirection(bool);

        void addPreviousMove(Point, State);
        
        Point suggestNextMove();
        Point suggestNextMove(DifficultyLevel);
};

#endif
