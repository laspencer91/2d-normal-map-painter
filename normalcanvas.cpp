// normalcanvas.cpp
#include "normalcanvas.h"
#include <QMouseEvent>
#include <QPainter>

NormalCanvas::NormalCanvas(QWidget *parent) : QOpenGLWidget(parent), normalMap(512, 512, QImage::Format_RGB32) {
    normalMap.fill(Qt::blue);
}

void NormalCanvas::setNormal(const QVector3D &normal) {
    currentNormal = normal;
}

void NormalCanvas::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);  // Ensure black background
    glClear(GL_COLOR_BUFFER_BIT);
}

void NormalCanvas::paintGL() {
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!baseImage.isNull()) {
        // Calculate the correct scaling to fit the canvas
        float aspectRatio = float(baseImage.width()) / float(baseImage.height());
        int displayWidth = width();
        int displayHeight = width() / aspectRatio;

        if (displayHeight > height()) {
            displayHeight = height();
            displayWidth = height() * aspectRatio;
        }

        int offsetX = (width() - displayWidth) / 2;
        int offsetY = (height() - displayHeight) / 2;

        imageRect = QRectF(offsetX, offsetY, displayWidth, displayHeight);

        // Draw the base image
        painter.drawImage(imageRect, baseImage);
    }

    // Draw the normal map on top with transparency
    painter.setOpacity(0.60);
    painter.drawImage(imageRect, normalMap);
}

void NormalCanvas::loadImage(const QString &filePath) {
    baseImage.load(filePath);  // Load the image
    std::printf("Loaded image %s\n", filePath.toStdString().c_str());

    if (!baseImage.isNull()) {
        normalMap = QImage(baseImage.size(), QImage::Format_RGB32);
        normalMap.fill(Qt::black);  // Clear normal map

        update();  // Trigger a redraw
    }
}

void NormalCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        const int x = event->pos().x();
        const int y = event->pos().y();
        if (x < normalMap.width() && y < normalMap.height()) {
            QColor color(
                static_cast<int>((currentNormal.x() + 1) * 127.5),
                static_cast<int>((currentNormal.y() + 1) * 127.5),
                static_cast<int>((currentNormal.z() + 1) * 127.5)
            );
            normalMap.setPixelColor(x, y, color);
            update();
        }
    }
}
