#include "gridwidget.h"


void GridWidget::init()
{
    this->playerBoard.setGridPositionStatus(3, 3, GridState::HIT);
    
    this->playerBoard.setGridPositionStatus(5, 5, GridState::SHIP);
    this->playerBoard.setGridPositionStatus(5, 6, GridState::SHIP);
    this->playerBoard.setGridPositionStatus(5, 7, GridState::SHIP);
    this->playerBoard.setGridPositionStatus(5, 8, GridState::SHIP);
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    if(event == nullptr)
    {
        return;
    }

    paintGrid(10, 50, playerBoard, "Player", Qt::blue);
    paintGrid(500, 50, opponentBoard, "Opponent", Qt::red);
}

void GridWidget::paintGrid(int x, int y, GridState _grid, QString header, QColor color)
{
    QPainter painter(this);

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
    auto grid = _grid;

    /* Player */
    QString player = header;
    QFont font=painter.font();
    font.setPointSize(28);
    painter.setFont(font);
    painter.setPen(textColor);
    painter.drawText(QPoint(baseLineX, baseLineY), player);

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
        auto l2_horistontal = QPointF(baseLineX + boardSize, baseLineY + smallOffset * (i*incrementFactor));
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
        auto l2_vertical = QPointF(baseLineX + smallOffset * (i*incrementFactor), baseLineY + boardSize);
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
            if(grid.getGridPositionStatus(i, j) == GridState::HIT)
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

            if(grid.getGridPositionStatus(i, j) == GridState::SHIP)
            {
                /* Circles - Lines */
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
