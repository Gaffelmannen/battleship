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

void Opponent::addPreviousMove(Point p, State s)
{
    p.state = s;
    moves.push_back(p);
    states.push_back(s);
    deleteFreeSquare(p);
}

Point Opponent::suggestNextMove()
{
    Point poa = suggestNextMove(selectedLevel);

    if(poa.x < 0)
        poa.x = 0;
    
    if(poa.x > 9)
        poa.x = 9;
    
    if(poa.y < 0)
        poa.y = 0;
    
    if(poa.y > 9)
        poa.y = 9;

    return poa;
}

void Opponent::deleteFreeSquare(Point toRemove)
{
    int index = -1;

    for(int i = 0; i < (int)this->freeSquares.size(); i++)
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
    bool isFree = false;

    for(int i = 0; i < (int)freeSquares.size(); i++)
    {
        if(p.x == freeSquares[i].x && p.y == freeSquares[i].y)
        {
            isFree = true;
            break;
        }
    }

    return isFree;
}

bool Opponent::wasLastAttackAHit()
{
    bool result = false;

    if(moves.size() <= 0)
        result = false;
    else if(states[states.size()-1] == State::HIT)
        result = true;
    
    return result;
}

bool Opponent::wasLastTwoAttacksHorisontal()
{
    bool result = false;

    if(moves.size() < 2)
        result = false;
    
    if(moves[moves.size()-2].x == moves[moves.size()-1].x)
        result = true;

    return result;
}

bool Opponent::wasLastThreeAttacksHorisontal()
{
    bool result = false;

    if(moves.size() < 3)
        result = false;
    
    if( moves[moves.size()-3].x ==
        moves[moves.size()-2].x == 
        moves[moves.size()-1].x)
        result = true;

    return result;
}

bool Opponent::isSquareHit(Point p)
{
    bool isHit = false;

    for(int i = 0; i < (int)moves.size(); i++)
    {
        if( p.x == moves[i].x && 
            p.y == moves[i].y &&
            p.state == State::HIT)
        {
            isHit = true;
            break;
        }
    }

    return isHit;
}

vector<Point> Opponent::getAllHitsFromMoves()
{
    vector<Point> hits = vector<Point>();

    for(int i = 0; i < (int)this->moves.size(); i++)
    {
        Point p = this->moves[i];
        State s = this->states[i];
        if(s == State::HIT)
        {
            hits.push_back(p);
        }
    }

    return hits;
}

Point Opponent::horisontalAttackOnShip(Point p)
{
    Point nextPointToAttack = randomAttackInTheMiddle();

    for(int i = p.x; i < boardSize; i++)
    {
        Point temp = Point(i, p.y);
        if(isSquareFree(temp))
        {
            nextPointToAttack = temp;
            break;
        }
    }
    
    return nextPointToAttack;
}

Point Opponent::verticalAttackOnShip(Point p)
{
    Point nextPointToAttack = randomAttackInTheMiddle();

    for(int i = p.y; i < boardSize; i++)
    {
        Point temp = Point(p.x, i);
        if(isSquareFree(temp))
        {
            nextPointToAttack = temp;
            break;
        }
    }

    return nextPointToAttack;
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

    vector<Point> verifiedMoves = vector<Point>();
    for (size_t i = 0; i < suggestions.size(); i++) 
    {
        for(size_t j = 0; j < this->freeSquares.size(); j++)
        {
            if (suggestions[i] == this->freeSquares[j]) 
            {
                verifiedMoves.push_back(suggestions[i]);
            }
        }
    }

    if(verifiedMoves.size() <= 0)
    {
        return this->randomAttackInTheMiddle();
    }

    return verifiedMoves[randomize(0, verifiedMoves.size()-1)];
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

Point Opponent::suggestNextMove(DifficultyLevel level)
{
    switch(level)
    {
        default:
        case DifficultyLevel::EASY:
        {
            Point pointOfAttack;

            int numberOfSquares = this->boardSize;
            int attackSquareX = randomize(0, numberOfSquares);
            int attackSquareY = randomize(0, numberOfSquares);
            pointOfAttack = Point(attackSquareX, attackSquareY);

            return pointOfAttack;
        }

        case DifficultyLevel::MEDIUM:
        {
            if(moves.size() <= 0)
            {
                return randomAttackInTheMiddle();
            }
            else
            {
                vector<Point> hits = getAllHitsFromMoves();

                if(hits.size() > 0)
                {
                    Point lastHit = hits[size(hits)-1];
                    return attackAreaAfterHit(lastHit);
                }
                else
                {
                    if(randomize())
                        return randomAttackInTheMiddle();
                    else
                        return attackAlongDiagonalLine();
                }
            }
        }

        case DifficultyLevel::HARD:
        {
            if(moves.size() <= 0)
            {
                return randomAttackInTheMiddle();
            }
            else
            {
                vector<Point> hits = getAllHitsFromMoves();

                if(wasLastTwoAttacksHorisontal())
                {
                    return horisontalAttackOnShip(hits[hits.size()-1]);
                }

                for(Point hit : hits)
                {
                    Point poa = randomAttackInTheMiddle();
                    
                    if(
                        wasLastTwoAttacksHorisontal() ||
                        (
                            !wasLastTwoAttacksHorisontal() &&
                            !wasLastAttackAHit()
                        )
                    )
                    {
                        poa = horisontalAttackOnShip(hit);
                    }
                    else if(
                        !wasLastTwoAttacksHorisontal() ||
                        (
                            wasLastTwoAttacksHorisontal() &&
                            !wasLastAttackAHit()
                        )
                    )
                    {
                        poa = verticalAttackOnShip(hit);
                    }

                    return poa;
                }
                
                if(randomize())
                    return randomAttackInTheMiddle();
                else
                    return attackAlongDiagonalLine();
            }
        }
    }
}
