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

    auto numberOfSquares = 12;
    auto incrementFactor = 4;
    auto boardSize = 450;

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
            painter.drawText(QPoint(20, 10 * (i*incrementFactor) - 10 ), text);
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
        painter.setPen(Qt::red);
        painter.drawLine(vertical_line);
    }
}

void GridWidget::foo()
{
    QPainter painter(this);
    auto l1 = QPointF(100, 500);
    auto l2 = QPointF(90, 90);
    auto w = 30; // rectangle width

    QLineF line = QLineF(l1, l2);
    QRectF rect = QRectF(0, 0, line.length(), w); // the rectangle will have the same width as the line's length
    painter.translate(line.p1()); // set the first point of the line as the origin
    painter.rotate(-line.angle()); // rotate the rectangle

    rect.setWidth(line.length());
    rect.setHeight(w);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::yellow);
    painter.drawRect(rect);

    painter.resetTransform();
    painter.setPen(Qt::red);
    painter.drawLine(line); // draw the line
    painter.drawEllipse(l1, 10,10); // draw the origin
}
