#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize

    MainWindow window;
    //window.showFullScreen();
    window.show();
    window.setMinimumWidth(800);
    window.setMinimumHeight(600);

    qDebug() << "Application started successfully.";

    return app.exec();  // Start the event loop
}
