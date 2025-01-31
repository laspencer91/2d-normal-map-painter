#include "previewcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>

PreviewCanvas::PreviewCanvas(QWidget *parent) : QOpenGLWidget(parent) {
    lightDirection = QVector3D(0.0f, 0.0f, 1.0f);  // Default front-facing light
}

QSize PreviewCanvas::sizeHint() const {
    int minSize = qMin(parentWidget()->width(), parentWidget()->height());
    minSize = qMax(minSize, 250);
    return QSize(minSize, minSize);  // Keep it square
}

void PreviewCanvas::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);  // Ensure black background
    glClear(GL_COLOR_BUFFER_BIT);
}

void PreviewCanvas::setImages(QImage *image, QImage *normalMap) {
    this->baseImage = image;
    this->normalMap = normalMap;
    update();  // Redraw the preview
}

void PreviewCanvas::setLightDirection(const QVector3D &direction) {
    lightDirection = direction.normalized();
    update();  // Update lighting effect
}

void PreviewCanvas::paintGL() {
    glClearColor(0, 0, 0, 1);  // Ensure black background
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::LosslessImageRendering);

    if (baseImage != nullptr && normalMap != nullptr && !baseImage->isNull() && !normalMap->isNull()) {
        // Scale image to fit canvas while maintaining aspect ratio
        float aspectRatio = static_cast<float>(baseImage->width()) / baseImage->height();
        int displayWidth = width();
        int displayHeight = width() / aspectRatio;

        if (displayHeight > height()) {
            displayHeight = height();
            displayWidth = height() * aspectRatio;
        }

        int offsetX = (width() - displayWidth) / 2;
        int offsetY = (height() - displayHeight) / 2;
        imageRect = QRectF(offsetX, offsetY, displayWidth, displayHeight);

        // Apply normal map shading dynamically
        QImage shadedImage = baseImage->copy();
        for (int y = 0; y < normalMap->height(); ++y) {
            for (int x = 0; x < normalMap->width(); ++x) {
                QColor normalColor = normalMap->pixelColor(x, y);
                QVector3D normal(
                    (normalColor.red() / 127.5f) - 1.0f,
                    (normalColor.green() / 127.5f) - 1.0f,
                    (normalColor.blue() / 127.5f) - 1.0f
                );
                normal.normalize();

                // Calculate light intensity (dot product of normal and light direction)
                float intensity = std::max(QVector3D::dotProduct(normal, lightDirection), 0.0f);
                QColor baseColor = baseImage->pixelColor(x, y);
                QColor shadedColor(
                    baseColor.red() * intensity,
                    baseColor.green() * intensity,
                    baseColor.blue() * intensity
                );
                shadedImage.setPixelColor(x, y, shadedColor);
            }
        }

        // Draw shaded image
        painter.drawImage(imageRect, shadedImage);
    }

    // Draw light handle
    painter.setBrush(Qt::red);
    painter.drawEllipse(handlePosition, 10, 10);
}

void PreviewCanvas::mousePressEvent(QMouseEvent *event) {
    handlePosition = event->pos();
    update();
}

void PreviewCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        handlePosition = event->pos();

        // Convert to normal space
        float nx = (handlePosition.x() - width() / 2.0f) / (width() / 2.0f);
        float ny = -(handlePosition.y() - height() / 2.0f) / (height() / 2.0f);
        float nz = sqrt(1.0f - qMin(1.0f, nx * nx + ny * ny));

        setLightDirection(QVector3D(nx, ny, nz));
    }
}

void PreviewCanvas::onNormalMapPaintAdjusted() {
    update();
}
