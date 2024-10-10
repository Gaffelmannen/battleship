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

class GridWidget : public QWidget
{
    private:
        int numberOfSquares;
        int incrementFactor;
        int boardSize;

        GridState gs;

    public:
        GridWidget()
        {
            numberOfSquares = 12;
            incrementFactor = 4;
            boardSize = 450;

            gs = GridState();
        };

    protected:
        void paintEvent(QPaintEvent*) override;
        void paintGrid();
};

#endif