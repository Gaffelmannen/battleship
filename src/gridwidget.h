#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#define DEBUG true

#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QLabel>
#include <QRect>
#include <QChar>

#include <random>
#include <iostream>

#include "shiptype.h"
#include "gridstate.h"
#include "point.h"

#define BOARD_SIZE 10

class GridWidget : public QWidget
{
    private:
        int numberOfSquares;
        int incrementFactor;
        int boardSize;

        GridState playerBoard;
        GridState opponentBoard;

    public:
        GridWidget() : 
            playerBoard(BOARD_SIZE),
            opponentBoard(BOARD_SIZE)
        {
            numberOfSquares = BOARD_SIZE;
            incrementFactor = 4;
            boardSize = 450;
            init();
        };

    protected:
        void init();
        int randomize(int, int);
        bool randomize();
        
        bool spawnShip(string, int);
        bool placeOpponentShip(ShipType*);

        void paintEvent(QPaintEvent*) override;
        void paintBackground();
        void paintGrid(int, int, GridState, QString, QColor);
};

#endif