#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // ReSharper disable CppDFAMemoryLeak - QWidgets and Components are managed by the parent.
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    sphere = new NormalSelectorSphere(this);
    canvas = new NormalCanvas(this);

    layout->addWidget(sphere, 1, Qt::AlignCenter);
    layout->addWidget(canvas, 1);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(sphere, &NormalSelectorSphere::normalSelected, canvas, &NormalCanvas::onSphereSelectedNormal);
    connect(canvas, &NormalCanvas::normalUpdated, sphere, &NormalSelectorSphere::onCanvasUpdatedNormal);
    connect(canvas, &NormalCanvas::sampledNormalChanged, sphere, &NormalSelectorSphere::onCanvasSampleUpdated);

    // ------------ Create a menu ----------------- //
    menuBar()->setNativeMenuBar(false);

    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), "Open Image", this);
    QAction *saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), "Save Normal Map", this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    // ------------ Menu Action Signals ----------------- //
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveNormalMap);
}

void MainWindow::openImage() {
    currentImageFilePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (!currentImageFilePath.isEmpty()) {
        canvas->loadImage(currentImageFilePath);  // Pass the file path to the canvas
    }
}

void MainWindow::saveNormalMap() {
    const QFileInfo fileInfo(currentImageFilePath);
    const QString newFilePath(fileInfo.baseName() + "/" + fileInfo.baseName() + "_normal");
    const QString filePath = QFileDialog::getSaveFileName(this, "Save Normal Map", newFilePath, "PNG Images (*.png);;JPEG Images (*.jpg)");
    if (!filePath.isEmpty()) {
        canvas->saveNormalMap(filePath);
    }
}

MainWindow::~MainWindow() {}