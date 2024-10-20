#include "gridwidget.h"

void GridWidget::init()
{
    gameover = false;
    playerHasNextTurn = true;
    numberOfTurnsPlayed = 0;

    opponentShips = addShipsToVector(true);
    playerShips = addShipsToVector(false);

    if(DEBUG)
    {
        cout << "Opponent ships created:" << endl;
        for(auto ship : opponentShips)
        {
            cout << ship->getName() << endl;
        }

        cout << "Player ships created:" << endl;
        for(auto ship : playerShips)
        {
            cout << ship->getName() << endl;
        }
    }
}

vector<ShipType*> GridWidget::addShipsToVector(bool isOpponentShip)
{
    auto s = vector<ShipType*>();
    s.push_back(spawnShip("Carrier", 5, isOpponentShip));
    s.push_back(spawnShip("Battleship", 4, isOpponentShip));
    s.push_back(spawnShip("Cruiser", 3, isOpponentShip));
    s.push_back(spawnShip("Submarine", 3, isOpponentShip));
    s.push_back(spawnShip("Destroyer", 2, isOpponentShip));
    return s;
}

void GridWidget::createActions()
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this);

    if(boxLayout == NULL)
    {
        return;
    }

    QMenu *menuGame = new QMenu("Game");
    menuGame->addAction("New", this, SLOT(newGame()));
    menuGame->addAction("Exi", this, SLOT(quit()));

    QMenu *menuHelp = new QMenu("Information");
    menuHelp->addAction(tr("Abou"), this, SLOT(about()));

    QMenuBar *mainMenu = new QMenuBar(this);
    mainMenu->addMenu(menuGame);
    mainMenu->addMenu(menuHelp);

    this->layout()->setMenuBar(mainMenu);
}

void GridWidget::about()
{
    if (DEBUG)
        cout << "About" << endl;
    
    QMessageBox::about(
        this, 
        tr("About Battlehip"),
        tr( "The <b>Application</b> is an implementation of the battleship game "
            "written as modern GUI applications using Qt and C++, "
            "it features a CPU opponent.")
    );
}

void GridWidget::quit()
{
    if (DEBUG)
        cout << "Exit" << endl;

    qApp->exit();
}

void GridWidget::newGame()
{
    if (DEBUG)
        cout << "New game" << endl;

    resetShips();
    init();
    repaint();
}

void GridWidget::opponentAttack()
{
    while(true)
    {
        Point pointOfAttack = opp.suggestNextMove();

        auto state = playerBoard.getGridPositionStatus(pointOfAttack);
        if(
            state == GridState::HIT ||
            state == GridState::MISS
        )
        {
            continue;
        }
        else if(state == GridState::FREE)
        {
            playerBoard.setGridPositionStatus(pointOfAttack, GridState::MISS);
            opp.addPreviousMove(pointOfAttack, GridState::MISS);
        }
        else if(state == GridState::SHIP)
        {
            playerBoard.setGridPositionStatus(pointOfAttack, GridState::HIT);
            opp.addPreviousMove(pointOfAttack, GridState::HIT);
        }

        for(auto ship : playerShips)
        {
            if(isShipSunk(ship, &playerBoard))
            {
                ship->sunk = true;
            }
        }

        if(checkIfOpponentWon())
        {
            QMessageBox::about(
                this, 
                tr("Lost"),
                tr((stringFormat("Battle lost! You lost the battle in %d turns.", numberOfTurnsPlayed).data()))
            );
            gameover = true;
        }
        else
        {
            playerHasNextTurn = true;
        }

        return;
    }
}

bool GridWidget::checkIfPlayerWon()
{
    bool winnerFound = false;
    
    int numberOfSunkenOpponentShips = 0;

    for(auto ship : opponentShips)
    {
        if(ship->sunk)
        {
            numberOfSunkenOpponentShips++;
        }
    }

    winnerFound = (int)opponentShips.size() == (int)numberOfSunkenOpponentShips;

    return winnerFound;
}

bool GridWidget::checkIfOpponentWon()
{
    bool winnerFound = false;
    
    int numberOfSunkenPlayerShips = 0;

    for(auto ship : playerShips)
    {
        if(ship->sunk)
        {
            numberOfSunkenPlayerShips++;
        }
    }

    winnerFound = (int)playerShips.size() == (int)numberOfSunkenPlayerShips;
    
    return winnerFound;
}

bool GridWidget::isShipSunk(ShipType* ship, GridState* board)
{
    vector<Point>* locations = ship->getLocation();
    
    bool isShipSunk = false;
    int numberOfHits = 0;

    if(DEBUG)
        cout << "Locations of " << ship->getName() << endl;
    
    for(auto loc : *locations)
    {
        Point current = Point(loc.x, loc.y);

        if(board->getGridPositionStatus(loc)==GridState::HIT)
        {
            numberOfHits++;
        }

        if(DEBUG)
            cout << 
                "X=" << 
                loc.x << 
                "      Y=" 
                << loc.y 
                << "    State="
                << opponentBoard.getGridPositionStatus(loc)
                << endl;
    }

    isShipSunk = (int)locations->size() == (int)numberOfHits;

    if(DEBUG)
        cout << "The ship is " << isShipSunk << endl;

    return isShipSunk;
}

int GridWidget::howManyShipsHasTheShipTaken(ShipType* ship, GridState* board)
{
    int numberOfHitsTheShipHasTaken = 0;

    auto locations = ship->getLocation();
    for(auto loc : *locations)
    {
        if(board->getGridPositionStatus(Point(loc.x, loc.y)) == GridState::HIT)
        {
            numberOfHitsTheShipHasTaken++;
        }
    }

    return numberOfHitsTheShipHasTaken;
}

void GridWidget::resetShips()
{
    for(int i = 0; i < numberOfSquares; i++)
    {
        for(int j = 0; j < numberOfSquares; j++)
        {
            opponentBoard.setGridPositionStatus(Point(i, j), GridState::FREE);
            playerBoard.setGridPositionStatus(Point(i, j), GridState::FREE);
        }
    }
}

ShipType* GridWidget::spawnShip(string type, int lengthOfShip, bool isOpponentShip)
{
    while(true)
    {
        bool vertical = randomize();
        auto points = new vector<Point>();
        
        int maxX = numberOfSquares - lengthOfShip;
        int maxY = numberOfSquares - lengthOfShip;

        int posX = randomize(0, maxX);
        int posY = randomize(0, maxY);
        
        for(int i = 0; i < lengthOfShip; i++)
        {
            if(vertical)
            {
                points->push_back(Point(posX, posY++));
            }
            else
            {
                points->push_back(Point(posX++, posY));
            }
            
            if(DEBUG)
                cout << "X:     " << posX << "      Y:      " << posY << endl;
        }
        
        ShipType* ship = new ShipType
        (
            type, 
            points
        );

        if(isOpponentShip)
        {
            if(placeShip(ship, isOpponentShip))
                return ship;
        }
        else
        {
            if(placeShip(ship, isOpponentShip))
                return ship;
        }
    }

    return NULL;
}

bool placePlayerShip(ShipType* ship)
{
    if(ship == NULL)
        return false;


    // Not yet implemented
    return false;
}

bool GridWidget::placeShip(ShipType* ship, bool isOpponent)
{
    for (auto point : *ship->getLocation())
    {
        if(isOpponent)
        {
            if(this->opponentBoard.getGridPositionStatus(point) != GridState::FREE)
            {
                return false;
            }
        }
        else
        {
            if(this->playerBoard.getGridPositionStatus(point) != GridState::FREE)
            {
                return false;
            }
        }
    }

    for (auto point : *ship->getLocation())
    {
        if(isOpponent)
        {
            this->opponentBoard.setGridPositionStatus(point, GridState::SHIP);
        }
        else
        {
            this->playerBoard.setGridPositionStatus(point, GridState::SHIP);
        }
    }

    return true;
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event == NULL)
        return;

    const QPoint clickPoint = event->pos();
    Point gridPoint = Point(-1, -1);

    if (false)
        cout << 
        "Mouse over     X=" << 
        clickPoint.x() << 
        "    Y=" << 
        clickPoint.y() << 
        endl;

    string board;
    int baseLineX;
    int baseLineY;

    if(
        clickPoint.x() >= 20 &&
        clickPoint.x() <= 20 + boardSize &&
        clickPoint.y() >= 50 &&
        clickPoint.y() <= 50 + boardSize
    )
    {
        board = "Player";
        baseLineX = 20;
        baseLineY = 50;
    }

    for(int i = 0; i < numberOfSquares; i++)
    {
        int gridTopX = baseLineX + (smallOffset * (i*incrementFactor));
        int gridTopY = baseLineY + (smallOffset * (i*incrementFactor));

        int gridBottomX = gridTopX + boxSize;
        int gridBottomY = gridTopY + boxSize;

        if(DEBUG)
        {
            cout << 
                "topX=" << 
                gridTopX << 
                "     toprY=" << 
                gridTopY << 
                "          bottomX=" << 
                gridBottomX <<
                "          bottomY=" <<
                gridBottomY <<
                endl;
        }

        if(gridTopX <= clickPoint.x() && clickPoint.x() >= gridBottomX)
        {
            gridPoint.x = i;
        }

        if(gridBottomY <= clickPoint.y() && clickPoint.y() >= gridBottomY)
        {
            gridPoint.y = i;
        }

        if(DEBUG)
        {
            cout << 
                "X=" << 
                gridPoint.x << 
                "     Y=" << 
                gridPoint.y << 
                endl;
        }
    }
}

void GridWidget::mousePressEvent(QMouseEvent * event)
{
    if(!playerHasNextTurn)
        return;

    const QPoint clickPoint = event->pos();
    Point gridPoint = Point(-1, -1);
    int baseLineX = -1;
    int baseLineY = -1;
    string board = "notset";

    if (DEBUG)
        cout << 
        "Mouse pressed     X=" << 
        clickPoint.x() << 
        "    Y=" << 
        clickPoint.y() << 
        endl;

    if(
        clickPoint.x() >= 510 &&
        clickPoint.x() <= 510 + boardSize &&
        clickPoint.y() >= 50 &&
        clickPoint.y() <= 50 + boardSize
    )
    {
        board = "Opponent";
        baseLineX = 510;
        baseLineY = 50;
    }
    else if(
        clickPoint.x() >= 20 &&
        clickPoint.x() <= 20 + boardSize &&
        clickPoint.y() >= 50 &&
        clickPoint.y() <= 50 + boardSize
    )
    {
        board = "Player";
        baseLineX = 20;
        baseLineY = 50;
    }

    if(board=="Player")
    {
        QMessageBox::about(
            this, 
            tr("Out of warzone"),
            tr("Please attack your opponent - rather than your own ships.")
        );
        return;
    }

    for(int i = 0; i < numberOfSquares; i++)
    {
        int gridTopX = baseLineX + (smallOffset * (i*incrementFactor));
        int gridTopY = baseLineY + (smallOffset * (i*incrementFactor));

        int gridBottomX = gridTopX + boxSize;
        int gridBottomY = gridTopY + boxSize;

        if(DEBUG)
        {
            cout << 
                "topX=" << 
                gridTopX << 
                "     toprY=" << 
                gridTopY << 
                "          bottomX=" << 
                gridBottomX <<
                "          bottomY=" <<
                gridBottomY <<
                endl;
        }

        if(gridTopX <= clickPoint.x() && clickPoint.x() >= gridBottomX)
        {
            gridPoint.x = i;
        }

        if(gridBottomY <= clickPoint.y() && clickPoint.y() >= gridBottomY)
        {
            gridPoint.y = i;
        }
    }

    if(board=="Opponent")
    {
        if(opponentBoard.getGridPositionStatus(gridPoint) == GridState::SHIP)
        {
            opponentBoard.setGridPositionStatus(gridPoint, GridState::HIT);
            numberOfTurnsPlayed++;
        }
        else if(opponentBoard.getGridPositionStatus(gridPoint) == GridState::FREE)
        {
            opponentBoard.setGridPositionStatus(gridPoint, GridState::MISS);
            numberOfTurnsPlayed++;
        }
        else
        {
            QMessageBox::about(
                this, 
                tr("Stike aborted"),
                tr("This square is already hit/missed.\nPlease select another to strike.")
            );
        }
    }
    
    if(DEBUG)
    {
        QMessageBox::about(
            this, 
            tr("Mouse click"),
            tr(stringFormat("Grid: %s \nPosition: X=%d, Y=%d", board.data(), gridPoint.x, gridPoint.y).data() )
        );
    }

    for(auto ship : opponentShips)
    {
        if(isShipSunk(ship, &opponentBoard) && ship->sunk == false)
        {
            ship->sunk = true;
            QMessageBox::about(
                this, 
                tr("Attack"),
                tr(stringFormat("Opponent - 'You sunk my %s'", ship->getName().data()).data())
            );
        }
    }

    if(checkIfPlayerWon())
    {
        QMessageBox::about(
            this, 
            tr("Won"),
            tr((stringFormat("Congratulations! You won in %d turns.", numberOfTurnsPlayed).data()))
        );

        gameover = true;
    }
    else
    {
        playerHasNextTurn = false;
        opponentAttack();
    }

    repaint();
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    if(event == nullptr)
    {
        return;
    }

    paintBackground();
    paintGrid(20, 50, playerBoard, "Player", Qt::blue);
    paintGrid(510, 50, opponentBoard, "Opponent", Qt::red);
    paintInfoBox();
}

void GridWidget::paintBackground()
{
    QPainter painter(this);
    painter.setOpacity(0.9);
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::black);
    painter.drawRect(
        10,
        10,
        1000,
        550
    );

    painter.drawRect(
        1020,
        10,
        170,
        550
    );
}

void GridWidget::paintInfoBox()
{
    int baseLineX = 1030;
    int baseLineY = 50;

    QPainter painter(this);
    
    QString header = "Info";
    QFont fontHeader=painter.font();
    fontHeader.setPointSize(28);
    painter.setFont(fontHeader);
    painter.setPen(Qt::darkBlue);
    painter.drawText(QPoint(baseLineX, baseLineY), header);

    QFont fontText=painter.font();
    fontText.setPointSize(12);
    painter.setFont(fontText);
    painter.setPen(Qt::white);

    QString turnsInfo = stringFormat("Turns %d", numberOfTurnsPlayed).data();
    painter.drawText(QPoint(baseLineX, baseLineY + 20), turnsInfo);

    QString playerShipInfo = "Player ships:";
    painter.drawText(QPoint(baseLineX, baseLineY + 40), playerShipInfo);

    int i = 1;
    baseLineY += 40;
    for(auto ship : playerShips)
    {
        if(ship == NULL)
            continue;

        string name = ship->getName();
        string status = " - is ";
        int numberOfHits = howManyShipsHasTheShipTaken(ship, &playerBoard);
        if(ship->sunk)
        {
            status += "sunk";
        }
        else
        {
            status += "floating";
        }
        QString shipInfo = ("" + name + stringFormat("(%d/%d)", numberOfHits, ship->getLocation()->size()) + status).data();

        painter.drawText(QPoint(baseLineX, baseLineY + 20 * i++), shipInfo);
    }

    baseLineY += 20*i;
    QString opponentShipInfo = "Opponent ships:";
    painter.drawText(QPoint(baseLineX, baseLineY + 40), opponentShipInfo);

    i = 1;
    for(auto ship : opponentShips)
    {
        if(ship == NULL)
            continue;

        string name = ship->getName();
        string status = " - is ";
        int numberOfHits = howManyShipsHasTheShipTaken(ship, &opponentBoard);
        if(ship->sunk)
        {
            status += "sunk";
        }
        else
        {
            status += "floating";
        }
        QString shipInfo = ("" + name + stringFormat("(%d/%d)", numberOfHits, ship->getLocation()->size()) + status).data();

        painter.drawText(QPoint(baseLineX, baseLineY + 40 + 20 * i++), shipInfo);
    }
}

void GridWidget::paintGrid(int x, int y, GridState _grid, QString _header, QColor color)
{
    QPainter painter(this);

    this->setAutoFillBackground(false);

    auto grid = _grid;

    auto textColor = color;
    auto gridColor = Qt::green;
    
    auto baseLineX = x;
    auto baseLineY = y;

    /* Header */
    QString header = _header;
    QFont font=painter.font();
    font.setPointSize(28);
    painter.setFont(font);
    painter.setPen(textColor);
    painter.drawText(QPoint(baseLineX, baseLineY), header);

    for (int i = 1; i < numberOfSquares + 2; i++)
    {
        /* Horisontal - Labels */
        if(i>1)
        {   
            QString original = "A";
            QChar c = original.at(0);
            int v_latin = c.toLatin1() + i - 2;
            QString text = QString(QChar(v_latin));
            QFont font=painter.font();
            font.setPointSize(18);
            painter.setFont(font);
            painter.setPen(textColor);
            painter.drawText(QPoint(baseLineX + smallOffset * (i*incrementFactor) - bigOffset, baseLineY + bigOffset + 5), text);
        }

        /* Horisontal - Lines */
        auto l1_horistontal = QPointF(baseLineX, baseLineY + smallOffset * (i*incrementFactor));
        auto l2_horistontal = QPointF(baseLineX + boardSize - smallOffset, baseLineY + smallOffset * (i*incrementFactor));
        QLineF horisontal_line = QLineF(l1_horistontal, l2_horistontal);
        painter.translate(horisontal_line.p1());
        painter.resetTransform();
        painter.setPen(gridColor);
        painter.drawLine(horisontal_line);

        /* Vertical - Labels */
        if(i>1)
        {
            QString original = "1";
            QChar c = original.at(0);
            int v_latin = c.toLatin1() + i - 2;
            QString text = QString(QChar(v_latin));
            if(text == ":")
                text = "10";
            QFont font=painter.font() ;
            font.setPointSize(18);
            painter.setFont(font);
            painter.setPen(textColor);
            painter.drawText(QPoint(baseLineX + smallOffset + 5, baseLineY + smallOffset * (i*incrementFactor) - 15), text);
        }

        /* Vertical - Lines */
        auto l1_vertical = QPointF(baseLineX + smallOffset * (i*incrementFactor), baseLineY + smallOffset);
        auto l2_vertical = QPointF(baseLineX + smallOffset * (i*incrementFactor), baseLineY + boardSize - smallOffset);
        QLineF vertical_line = QLineF(l1_vertical, l2_vertical);
        painter.translate(vertical_line.p1());
        painter.resetTransform();
        painter.setPen(gridColor);
        painter.drawLine(vertical_line);

        if(DEBUG)
        {
            cout << 
                "verX=" << 
                l1_vertical.x() << 
                "     verY=" << 
                l1_vertical.y() << 
                "          horX=" << 
                l2_horistontal.x() <<
                "          horY=" <<
                l2_horistontal.y() <<
                endl;

        }
    }

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(DEBUG || header == "Player")
            {
                if(
                    grid.getGridPositionStatus(Point(i, j)) == GridState::SHIP ||
                    grid.getGridPositionStatus(Point(i, j)) == GridState::HIT
                )
                {
                    /* Ships */
                    painter.setPen(Qt::darkGray);
                    painter.setBrush(Qt::gray);
                    painter.drawRect(
                        baseLineX + boxOffset + (smallOffset * i * incrementFactor), 
                        baseLineY + boxOffset + (smallOffset * j * incrementFactor), 
                        boxSize, 
                        boxSize
                    );
                }
            }

            if(grid.getGridPositionStatus(Point(i, j)) == GridState::HIT)
            {
                /* Circles - Lines */
                painter.setBrush(Qt::white);
                painter.drawEllipse(QPointF(
                    baseLineX + circleOffset + (smallOffset * i * incrementFactor), 
                    baseLineY + circleOffset + (smallOffset * j * incrementFactor)), 
                    circleRadius, 
                    circleRadius
                );
            }

            if(grid.getGridPositionStatus(Point(i, j)) == GridState::MISS)
            {
                /* Circles - Lines */
                painter.setBrush(Qt::red);
                painter.drawEllipse(QPointF(
                    baseLineX + circleOffset + (smallOffset * i * incrementFactor), 
                    baseLineY + circleOffset + (smallOffset * j * incrementFactor)), 
                    circleRadius, 
                    circleRadius
                );
            }
        }
    }
}
