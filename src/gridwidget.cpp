#include "gridwidget.h"

void GridWidget::init()
{
    spawnShip("Carrier", 5);
    spawnShip("Battleship", 4);
    spawnShip("Cruiser", 3);
    spawnShip("Submarine", 3);
    spawnShip("Destroyer", 2);
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
    menuHelp->addAction("Abou", this, SLOT(about()));

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
    qApp->exit();
}

void GridWidget::newGame()
{
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

bool GridWidget::spawnShip(string type, int lengthOfShip)
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
            return true;
    }

    return true;
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

    auto smallOffset = 10;
    auto bigOffset = 25;
    auto circleRadius = 20;
    auto circleOffset = 60;
    auto boxOffset = 40;
    auto boxSize = 40;

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
    }

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(grid.getGridPositionStatus(Point(i, j)) == GridState::HIT)
            {
                /* Circles - Lines */
                painter.setBrush(Qt::yellow);
                painter.drawEllipse(QPointF(
                    baseLineX + circleOffset + (smallOffset * i * incrementFactor), 
                    baseLineY + circleOffset + (smallOffset * j * incrementFactor)), 
                    circleRadius, 
                    circleRadius
                );
            }
            
            if(DEBUG)
            {
                if(grid.getGridPositionStatus(Point(i, j)) == GridState::SHIP)
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
        }
    }
}
