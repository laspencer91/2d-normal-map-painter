// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // ReSharper disable CppDFAMemoryLeak - QWidgets and Components are managed by the parent.
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    sphere = new NormalSphere(this);
    canvas = new NormalCanvas(this);

    layout->addWidget(sphere, 1, Qt::AlignCenter);
    layout->addWidget(canvas, 1);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(sphere, &NormalSphere::normalChanged, canvas, &NormalCanvas::setNormal);

    // Create a menu -----------------
    menuBar()->setNativeMenuBar(false);
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *openAction = new QAction("Open Image", this);
    fileMenu->addAction(openAction);

    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);
}

void MainWindow::openImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        canvas->loadImage(filePath);  // Pass the file path to the canvas
    }
}

MainWindow::~MainWindow() {}