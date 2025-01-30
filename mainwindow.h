// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "normalcanvas.h"
#include "normalsphere.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    NormalCanvas *canvas;
    NormalSphere *sphere;

private slots:
    void openImage();  // New slot for loading images

};

#endif // MAINWINDOW_H
