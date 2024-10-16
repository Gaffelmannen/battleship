#include "gridwidget.h"

template<typename ... Args>
std::string GridWidget::stringFormat(const std::string& format, Args ... args)
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ...) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ...);
    return std::string( buf.get(), buf.get() + size - 1 );
}

void GridWidget::init()
{
    ships = vector<ShipType*>();

    ships.push_back(spawnShip("Carrier", 5));
    ships.push_back(spawnShip("Battleship", 4));
    ships.push_back(spawnShip("Cruiser", 3));
    ships.push_back(spawnShip("Submarine", 3));
    ships.push_back(spawnShip("Destroyer", 2));

    if(DEBUG)
    {
        cout << "Opponent ships created:" << endl;
        for(auto ship : ships)
        {
            cout << ship->getName() << endl;
        }
    }
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

int GridWidget::randomize(int low, int high)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(low, high);

    return (int)dist(mt);
}

bool GridWidget::randomize()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    auto value = dist(mt);
    return (int)value % 2 == 0;
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

    isShipSunk = locations->size() == numberOfHits;

    if(DEBUG)
        cout << "The ship is " << isShipSunk << endl;

    return isShipSunk;
}

void GridWidget::resetShips()
{
    for(int i = 0; i < numberOfSquares; i++)
    {
        for(int j = 0; j < numberOfSquares; j++)
        {
            opponentBoard.setGridPositionStatus(Point(i, j), GridState::FREE);
        }
    }
}

ShipType* GridWidget::spawnShip(string type, int lengthOfShip)
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

        if(placeOpponentShip(ship))
            return ship;
    }

    return NULL;
}

bool GridWidget::placeOpponentShip(ShipType* ship)
{
    for (auto point : *ship->getLocation())
    {
        if(this->opponentBoard.getGridPositionStatus(point) != GridState::FREE)
        {
            return false;
        }
    }

    for (auto point : *ship->getLocation())
    {
        this->opponentBoard.setGridPositionStatus(point, GridState::SHIP);
    }

    return true;
}

void GridWidget::mousePressEvent (QMouseEvent * event)
{
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

    for(int i = 0; i < numberOfSquares; i++)
    {
        int gridTopX = baseLineX + (smallOffset * (i*incrementFactor));
        int gridTopY = baseLineY + (smallOffset * (i*incrementFactor));

        int gridBottomX = gridTopX + boxSize;
        int gridBottomY = gridTopY + boxSize;

        if(false)
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
        }
        else if(opponentBoard.getGridPositionStatus(gridPoint) == GridState::FREE)
        {
            opponentBoard.setGridPositionStatus(gridPoint, GridState::MISS);
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
    
    if(false)
    {
        QMessageBox::about(
            this, 
            tr("Mouse click"),
            tr(stringFormat("Grid: %s \nPosition: X=%d, Y=%d", board.data(), gridPoint.x, gridPoint.y).data() )
        );
    }

    for(auto ship : ships)
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
}

void GridWidget::paintBackground()
{
    QPainter painter(this);
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::black);
    painter.drawRect(
        10,
        10,
        1000,
        550
    );
}

void GridWidget::paintGrid(int x, int y, GridState _grid, QString _header, QColor color)
{
    QPainter painter(this);

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

        if(false)
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
            if(DEBUG)
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
