#include "gridwidget.h"

#include <iostream>

int main(int argc, char *argv[])
{
    auto imageWidth = 1200;
    auto aspectRatio = 16.0 / 9.0;
    auto imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    if(DEBUG)
    {
        cout << "Resolution " << imageWidth << "x" << imageHeight << endl;
    }

    QApplication app(argc, argv);

    QIcon icon(".res/icon.ico"); 
    app.setWindowIcon(QIcon("./res/icon.png"));
    app.setApplicationVersion(APP_VERSION);

    QSize s = QSize(imageWidth, imageHeight);
    GridWidget widget;
    //QWidget widget;
    widget.setObjectName("GameWindow");
    widget.setStyleSheet("#GameWindow { background-image: url(res/bg.jpeg); }");
    widget.setMouseTracking(true);
    widget.resize(s);
    widget.setMinimumSize(s);
    widget.setMaximumSize(s);
    widget.show();

    QFont font = QFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(font);


    return app.exec();
}

