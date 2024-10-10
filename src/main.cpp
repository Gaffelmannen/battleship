#include "gridwidget.h"

int main(int argc, char *argv[])
{
    auto imageWidth = 900;
    auto aspectRatio = 16.0 / 9.0;
    auto imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    QApplication app(argc, argv);
    GridWidget widget;
    widget.resize(QSize(imageWidth, imageHeight));
    widget.show();
    return app.exec();
}
