// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NormalCanvas.h"
#include "NormalSelectorSphere.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    NormalCanvas *canvas;
    NormalSelectorSphere *sphere;
    QString currentImageFilePath;

private slots:
    void openImage();  // New slot for loading images
    void saveNormalMap();
};

#endif // MAINWINDOW_H
