#include "gridwidget.h"

void GridWidget::paintEvent(QPaintEvent* event)
{
    if(event == nullptr)
    {
        return;
    }

    paintGrid();
}

void GridWidget::paintGrid()
{
    QPainter painter(this);

    for (int i = 1; i < numberOfSquares; i++)
    {
        if(i>1)
        {
            QString original = "A";
            QChar c = original.at(0);
            int v_latin = c.toLatin1() + i - 2;
            QString text = QString(QChar(v_latin));
            QFont font=painter.font() ;
            font.setPointSize(18);
            painter.setFont(font);
            painter.setPen(Qt::yellow);
            painter.drawText(QPoint(20, 10 * (i*incrementFactor) - 14 ), text);
        }

        auto l1_horistontal = QPointF(10, 10 * (i*incrementFactor));
        auto l2_horistontal = QPointF(boardSize, 10 * (i*incrementFactor));
        QLineF horisontal_line = QLineF(l1_horistontal, l2_horistontal);
        painter.translate(horisontal_line.p1());
        painter.resetTransform();
        painter.setPen(Qt::green);
        painter.drawLine(horisontal_line);

        if(i>1)
        {   
            QString original = "1";
            QChar c = original.at(0);
            int v_latin = c.toLatin1() + i - 2;
            QString text = QString(QChar(v_latin));
            if(text == ":")
                text = "10";
            QFont font=painter.font();
            font.setPointSize(18);
            painter.setFont(font);
            painter.setPen(Qt::yellow);
            painter.drawText(QPoint(10 * (i*incrementFactor) - 25, 25), text);
        }

        auto l1_vertical = QPointF(10 * (i*incrementFactor), 10);
        auto l2_vertical = QPointF(10 * (i*incrementFactor), boardSize);
        QLineF vertical_line = QLineF(l1_vertical, l2_vertical);
        painter.translate(vertical_line.p1());
        painter.resetTransform();
        painter.setPen(Qt::green);
        painter.drawLine(vertical_line);
    }
}
