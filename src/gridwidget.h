#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QLabel>
#include <QRect>
#include <QChar>

#include "gridstate.h"

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
        };

    protected:
        void paintEvent(QPaintEvent*) override;
        void paintGrid(int, int, GridState);
};

#endif