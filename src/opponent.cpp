#include "opponent.h"
#include "misc.h"

void Opponent::setDiagonalAttackDirection(bool startTopLeft)
{
    this->diagonalLineAttackStartTopleft = startTopLeft;

    if(startTopLeft)
    {
        diagonalLinePosition = 0;
    }
    else
    {
        diagonalLinePosition = 9;
    }
}

void Opponent::addPreviousMove(Point p, GridState::State s)
{
    moves.push_back(p);
    states.push_back(s);
    deleteFreeSquare(p);
}

Point Opponent::suggestNextMove()
{
    Point poa = Point(-1, -1);
    switch (this->selectedLevel)
    {
        case SIMPLE:
        case EASY:
            poa = suggestNextLevel1Move();
            break;

        case MEDIUM:
            poa = suggestNextLevel2Move();
            break;

        default:
            break;
    }

    return poa;
}

void Opponent::deleteFreeSquare(Point toRemove)
{
    int index = -1;

    for(int i = 0; i < this->freeSquares.size(); i++)
    {
        if(
            freeSquares[i].x == toRemove.x &&
            freeSquares[i].y == toRemove.y
        )
        {
            index = i;
            break;
        }
    }

    freeSquares.erase(freeSquares.begin() + index);
}

void Opponent::setupFreeSquares()
{
    for(int i = 0; i < this->boardSize; i++)
    {
        for(int j = 0; j < this->boardSize; j++)
        {
            Point p = Point(i, j);
            freeSquares.push_back(p);
        }
    }
}

bool Opponent::isSquareFree(Point p)
{
    bool isFree = true;

    for(int i = 0; i < freeSquares.size(); i++)
    {
        if(p.x == freeSquares[i].x && p.y == freeSquares[i].y)
        {
            isFree = false;
            break;
        }
    }

    return isFree;
}

Point Opponent::attackRandomFreeSquare()
{
    cout << "Attack random free square" << endl;
    int indexToAttack = randomize(0, freeSquares.size());
    return Point(freeSquares[indexToAttack]);
}

Point Opponent::attackAreaAfterHit(Point lastMove)
{
    cout << "Attack area after hit" << endl;

    vector<Point> suggestions = vector<Point>();
    suggestions.push_back(Point(lastMove.x-1, lastMove.y+1));
    suggestions.push_back(Point(lastMove.x, lastMove.y+1));
    suggestions.push_back(Point(lastMove.x+1, lastMove.y+1));
    suggestions.push_back(Point(lastMove.x-1, lastMove.y));
    suggestions.push_back(Point(lastMove.x+1, lastMove.y));
    suggestions.push_back(Point(lastMove.x-1, lastMove.y-1));
    suggestions.push_back(Point(lastMove.x, lastMove.y-1));
    suggestions.push_back(Point(lastMove.x+1, lastMove.y-1));

    return suggestions[randomize(0, suggestions.size()-1)];
}

Point Opponent::randomAttackInTheMiddle()
{
    cout << "Random attack in the middle" << endl;
    if(moves.size() < 9)
    {
        return Point(
                randomize(4, this->boardSize-3),
                randomize(4, this->boardSize-3)
            );
    }
    else if(moves.size() < 20)
    {
        return Point(
            randomize(3, this->boardSize-2),
            randomize(3, this->boardSize-2)
        );
    }
    else if(moves.size() < 30)
    {
        return Point(
            randomize(2, this->boardSize-1),
            randomize(2, this->boardSize-1)
        );
    }
    else if(moves.size() < 40 && moves.size() < 50)
    {
        return Point(
                randomize(0, this->boardSize),
                randomize(0, this->boardSize)
            );
    }
    else{
        return attackRandomFreeSquare();
    }
}

Point Opponent::attackAlongDiagonalLine()
{
    cout << "Attack along diagonal line" << endl;
    Point p;

    if(this->diagonalLineAttackStartTopleft)
    {
        p.x = diagonalLinePosition;
        p.y = diagonalLinePosition;
        diagonalLinePosition++;
    }
    else
    {
        p.x = diagonalLinePosition;
        p.y = diagonalLinePosition;
        diagonalLinePosition--;
    }

    if(!isSquareFree(p))
    {
        p = attackRandomFreeSquare();
    }

    return p;
}

Point Opponent::suggestNextLevel1Move()
{
    Point pointOfAttack;

    int numberOfSquares = this->boardSize;
    int attackSquareX = randomize(0, numberOfSquares);
    int attackSquareY = randomize(0, numberOfSquares);
    pointOfAttack = Point(attackSquareX, attackSquareY);

    return pointOfAttack;
}

Point Opponent::suggestNextLevel2Move()
{
    if(moves.size() <= 0)
    {
        return randomAttackInTheMiddle();
    }
    else
    {
        Point nextMove;

        GridState::State lastState = states[states.size()-1];
        Point lastMove = moves[moves.size()-1];
        cout << "PreviusMove   X=" << lastMove.x << " Y=" << lastMove.y << "   Outcome=" << lastState << "  FreeSquares=" << freeSquares.size() << endl;

        if(lastState == GridState::HIT)
        {
            return attackAreaAfterHit(lastMove);
        }
        else
        {
            if(randomize())
                return randomAttackInTheMiddle();
            else
                return attackAlongDiagonalLine();
        }

        if(nextMove.x < 0)
            nextMove.x = 0;
        
        if(nextMove.x > 9)
            nextMove.x = 9;
        
        if(nextMove.y < 0)
            nextMove.y = 0;
        
        if(nextMove.y > 9)
            nextMove.y = 9;

        return nextMove;
    }
}