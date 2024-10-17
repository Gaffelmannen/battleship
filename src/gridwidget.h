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
#include <QMenuBar>
#include <QVBoxLayout>
#include <QMessageBox>

#include <memory>
#include <string>

#include "shiptype.h"
#include "gridstate.h"
#include "point.h"


#define NUMBER_OF_GRIDS_ON_THE_BOARD 10

class GridWidget : public QWidget
{
    Q_OBJECT

    private:
        int numberOfSquares;
        int incrementFactor;
        int boardSize;

        int smallOffset = 10;
        int bigOffset = 25;
        int circleRadius = 20;
        int circleOffset = 60;
        int boxOffset = 40;
        int boxSize = 40;

        int numberOfTurnsPlayed;

        vector<ShipType*> opponentShips;
        GridState playerBoard;
        GridState opponentBoard;

    public:
        GridWidget() : 
            playerBoard(NUMBER_OF_GRIDS_ON_THE_BOARD),
            opponentBoard(NUMBER_OF_GRIDS_ON_THE_BOARD)
        {
            numberOfSquares = NUMBER_OF_GRIDS_ON_THE_BOARD;
            incrementFactor = 4;
            boardSize = 450;

            setWindowTitle(tr("Battleship"));
            
            createActions();

            init();
        };

    protected:

        void init();
        void createActions();
        
        bool checkForWinner();
        bool isShipSunk(ShipType*, GridState*);
        void resetShips();
        ShipType* spawnShip(string, int);
        bool placeOpponentShip(ShipType*);

        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent*);
        void paintEvent(QPaintEvent*) override;
        void paintBackground();
        void paintGrid(int, int, GridState, QString, QColor);

    private slots:
        void about();
        void quit();
        void newGame();
};

#endif