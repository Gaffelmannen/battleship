#include <QApplication>
#include <QPainter>

class MyWidget : public QWidget{
public:
    MyWidget(){

    }

protected:
    void paintEvent(QPaintEvent* event) override{
        QPainter painter(this);
        auto l1 = QPointF(100,100);
        auto l2 = QPointF(500, 800);

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
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWidget widget;
    widget.resize(QSize(1000,1000));
    widget.show();
    return app.exec();
}

