// normalcanvas.cpp
#include "normalcanvas.h"
#include <QMouseEvent>
#include <QPainter>

NormalCanvas::NormalCanvas(QWidget *parent) : QOpenGLWidget(parent), normalMap(512, 512, QImage::Format_RGB32) {
    normalMap.fill(Qt::blue);
    setMouseTracking(true);
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
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::LosslessImageRendering);

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
    const int screenX = event->pos().x();
    const int screenY = event->pos().y();

    if (event->buttons() & Qt::LeftButton) {
        applyPaint(screenX, screenY);
    } else if (event->buttons() & Qt::RightButton) {
        removePaint(screenX, screenY);
    } else {
        sampleNormalAt(screenX, screenY);
    }
}

void NormalCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        const int screenX = event->pos().x();
        const int screenY = event->pos().y();
        applyPaint(screenX, screenY);
    } else if (event->buttons() & Qt::RightButton) {
        const int screenX = event->pos().x();
        const int screenY = event->pos().y();
        removePaint(screenX, screenY);
    }
}

void NormalCanvas::applyPaint(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        // Convert screen coordinates to image space (normalized 0-1)
        float normalizedX = (screenX - imageRect.x()) / imageRect.width();
        float normalizedY = (screenY - imageRect.y()) / imageRect.height();

        // Scale to image pixel space
        int imageX = static_cast<int>(normalizedX * normalMap.width());
        int imageY = static_cast<int>(normalizedY * normalMap.height());

        // Ensure we stay in valid pixel range
        imageX = std::clamp(imageX, 0, normalMap.width() - 1);
        imageY = std::clamp(imageY, 0, normalMap.height() - 1);

        // Convert normal vector to RGB color
        QColor color(
            static_cast<int>((currentNormal.x() + 1) * 127.5),
            static_cast<int>((currentNormal.y() + 1) * 127.5),
            static_cast<int>((currentNormal.z() + 1) * 127.5)
        );

        // Apply to the correct pixel in the normal map
        normalMap.setPixelColor(imageX, imageY, color);

        // Force a repaint
        update();
    }
}

void NormalCanvas::removePaint(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        // Convert screen coordinates to image space (normalized 0-1)
        float normalizedX = (screenX - imageRect.x()) / imageRect.width();
        float normalizedY = (screenY - imageRect.y()) / imageRect.height();

        // Scale to image pixel space
        int imageX = static_cast<int>(normalizedX * normalMap.width());
        int imageY = static_cast<int>(normalizedY * normalMap.height());

        // Ensure we stay in valid pixel range
        imageX = std::clamp(imageX, 0, normalMap.width() - 1);
        imageY = std::clamp(imageY, 0, normalMap.height() - 1);

        // Apply to the correct pixel in the normal map
        normalMap.setPixelColor(imageX, imageY, Qt::black);

        // Force a repaint
        update();
    }
}

void NormalCanvas::sampleNormalAt(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        // Convert screen coordinates to image space (normalized 0-1)
        float normalizedX = (screenX - imageRect.x()) / imageRect.width();
        float normalizedY = (screenY - imageRect.y()) / imageRect.height();

        // Scale to image pixel space
        int imageX = static_cast<int>(normalizedX * normalMap.width());
        int imageY = static_cast<int>(normalizedY * normalMap.height());

        // Ensure we stay in valid pixel range
        imageX = std::clamp(imageX, 0, normalMap.width() - 1);
        imageY = std::clamp(imageY, 0, normalMap.height() - 1);

        // Apply to the correct pixel in the normal map
        QColor sampledColor = normalMap.pixelColor(imageX, imageY);
        if (sampledColor != previousSampledColor && sampledColor != Qt::black) {
            float nx = (sampledColor.red() / 127.5f) - 1.0f;
            float ny = (sampledColor.green() / 127.5f) - 1.0f;
            float nz = (sampledColor.blue() / 127.5f) - 1.0f;

            QVector3D sampledNormal = QVector3D(nx, ny, nz).normalized();

            emit sampledNormalChanged(sampledNormal);
            previousSampledColor = sampledColor;
        }
    }
}
