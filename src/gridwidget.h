#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QLabel>
#include <QRect>
#include <QChar>

class GridWidget : public QWidget
{
    public:
        GridWidget() {}

    protected:
        void paintEvent(QPaintEvent*) override;
        void paintGrid();
        void foo();

};