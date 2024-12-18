#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#define DEBUG false

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

#include <iostream>
#include <memory>
#include <string>

#include "misc.h"
#include "shiptype.h"
#include "gridstate.h"
#include "point.h"
#include "opponent.h"

#define NUMBER_OF_GRIDS_ON_THE_BOARD 10

class GridWidget : public QWidget
{
    Q_OBJECT

    private:
        int numberOfSquares;
        int incrementFactor;
        int boardSize;
        int opponentDifficulty;

        int smallOffset = 10;
        int bigOffset = 25;
        int circleRadius = 20;
        int circleOffset = 60;
        int boxOffset = 40;
        int boxSize = 40;

        bool gameover;
        bool playerHasNextTurn;
        int numberOfTurnsPlayed;

        vector<ShipType*> opponentShips;
        vector<ShipType*> playerShips;

        GridState playerBoard;
        GridState opponentBoard;

        Opponent opp = Opponent(NUMBER_OF_GRIDS_ON_THE_BOARD, Opponent::MEDIUM);

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
        vector<ShipType*> addShipsToVector(bool);
        void createActions();

        void opponentAttack();
        bool checkIfPlayerWon();
        bool checkIfOpponentWon();
        bool isShipSunk(ShipType*, GridState*);
        int howManyShipsHasTheShipTaken(ShipType*, GridState*);
        void resetShips();
        ShipType* spawnShip(string, int, bool);
        bool placePlayerShip(ShipType*);
        bool placeShip(ShipType*, bool);

        void mouseMoveEvent(QMouseEvent *) override;
        void mousePressEvent(QMouseEvent*) override;
        void paintEvent(QPaintEvent*) override;
        void paintBackground();
        void paintInfoBox();
        void paintGrid(int, int, GridState, QString, QColor);

    private slots:
        void about();
        void quit();
        void newGame();
};

#endif