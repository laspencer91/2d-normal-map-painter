#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize

    MainWindow window;
    //window.showFullScreen();
    window.setWindowTitle("2D Normal Map Painter");
    window.show();
    window.setMinimumWidth(800);
    window.setMinimumHeight(600);

    qDebug() << "Application started successfully.";

    return app.exec();  // Start the event loop
}
