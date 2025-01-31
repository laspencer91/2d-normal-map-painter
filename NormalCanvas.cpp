#include "NormalCanvas.h"
#include <QMouseEvent>
#include <QPainter>

NormalCanvas::NormalCanvas(QWidget *parent) : QOpenGLWidget(parent), normalMap(512, 512, QImage::Format_RGB32) {
    normalMap.fill(Qt::transparent);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void NormalCanvas::setNormal(const QVector3D &normal) {
    currentNormal = normal;
    emit normalUpdated(normal);
}

void NormalCanvas::onSphereSelectedNormal(const QVector3D &normal) {
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
        // Calculate size of the image
        const float aspectRatio = static_cast<float>(baseImage.width()) / static_cast<float>(baseImage.height());
        int containerWidth = width() * scaleFactor;
        int containerHeight = width() / aspectRatio * scaleFactor;

        // Calculate offset for centered image placement
        const int offsetForCenterX = (width() - containerWidth) / 2;
        const int offsetForCenterY = (height() - containerHeight) / 2;

        imageRect = QRectF(offsetForCenterX + imagePan.x(), offsetForCenterY + imagePan.y(), containerWidth, containerHeight);

        painter.drawImage(imageRect, baseImage);
    }

    // Draw the normal map on top with transparency
    painter.setOpacity(0.60);
    painter.drawImage(imageRect, normalMap);
}

void NormalCanvas::loadImage(const QString &filePath) {
    baseImage.load(filePath);  // Load the image
    std::printf("Loaded image %s\n", filePath.toStdString().c_str());

    setNormal(QVector3D(0, 0, 1));
    imagePan = QPointF(0, 0);

    if (!baseImage.isNull()) {
        normalMap = QImage(baseImage.size(), QImage::Format_RGB32);
        normalMap.fill(Qt::black);  // Clear normal map
        fitImageInCanvas();
        update();  // Trigger a redraw
    }
}

void NormalCanvas::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        setNormal(currentSampledNormal);
    }
}

void NormalCanvas::mouseMoveEvent(QMouseEvent *event) {
    const int screenX = event->pos().x();
    const int screenY = event->pos().y();

    if (event->buttons() & Qt::LeftButton) {
        applyPaint(screenX, screenY);
    } else if (event->buttons() & Qt::RightButton) {
        removePaint(screenX, screenY);
    } else if (event->buttons() & Qt::MiddleButton) {
        QPointF mouseDelta =  event->position() - lastMousePos;
        imagePan += mouseDelta;
        update();
    } else {
        sampleNormalAt(screenX, screenY);
    }

    lastMousePos = event->position();
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

void NormalCanvas::wheelEvent(QWheelEvent *event) {
    // Handle Mouse Wheel Scaling
    const float wheelAngleChange = event->angleDelta().y() / 8;     // Why is the true angle is multiplied by 8?
    const float angleSign = wheelAngleChange > 0.0f ? 1.0f : -1.0f; // The sign of the angle.
    scaleFactor += angleSign * scaleAdjustmentStep;
    scaleFactor = std::clamp(scaleFactor, 0.1f, 50.0f);
    update();                                                       // Trigger Repaint
}

void NormalCanvas::applyPaint(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        QPoint imagePos = convertScreenToImage(screenX, screenY);

        // Convert normal vector to RGB color
        QColor color(
            static_cast<int>((currentNormal.x() + 1) * 127.5),
            static_cast<int>((currentNormal.y() + 1) * 127.5),
            static_cast<int>((currentNormal.z() + 1) * 127.5)
        );

        normalMap.setPixelColor(imagePos.x(), imagePos.y(), color);

        // Force a repaint
        update();
    }
}

void NormalCanvas::removePaint(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        QPoint imagePos = convertScreenToImage(screenX, screenY);

        // Apply to the correct pixel in the normal map
        normalMap.setPixelColor(imagePos.x(), imagePos.y(), Qt::black);

        // Force a repaint
        update();
    }
}

void NormalCanvas::sampleNormalAt(int screenX, int screenY) {
    if (imageRect.contains(screenX, screenY)) {
        QPoint imagePos = convertScreenToImage(screenX, screenY);

        QColor sampledColor = normalMap.pixelColor(imagePos.x(), imagePos.y());
        if (sampledColor != previousSampledColor && sampledColor != Qt::black) {
            float nx = (sampledColor.red() / 127.5f) - 1.0f;
            float ny = (sampledColor.green() / 127.5f) - 1.0f;
            float nz = (sampledColor.blue() / 127.5f) - 1.0f;

            currentSampledNormal = QVector3D(nx, ny, nz).normalized();

            emit sampledNormalChanged(currentSampledNormal);
            previousSampledColor = sampledColor;
        }
    }
}

QPoint NormalCanvas::convertScreenToImage(int screenX, int screenY) const {
    // Convert screen coordinates to image space (normalized 0-1)
    float normalizedX = (screenX - imageRect.x()) / imageRect.width();
    float normalizedY = (screenY - imageRect.y()) / imageRect.height();

    // Scale to image pixel space
    int imageX = static_cast<int>(normalizedX * normalMap.width());
    int imageY = static_cast<int>(normalizedY * normalMap.height());

    // Ensure we stay in valid pixel range
    imageX = std::clamp(imageX, 0, normalMap.width() - 1);
    imageY = std::clamp(imageY, 0, normalMap.height() - 1);

    return QPoint(imageX, imageY);
}

void NormalCanvas::fitImageInCanvas() {
    const float aspectRatio = static_cast<float>(baseImage.width()) / static_cast<float>(baseImage.height());
    float containerWidth = width();
    float containerHeight = width() / aspectRatio;
    if (containerHeight > height()) {
        scaleFactor = height() / containerHeight;
        if (containerWidth * scaleFactor > containerWidth) {
            scaleFactor = width() / containerWidth;
        }
    } else if (containerWidth > width()) {
        scaleFactor = width() / containerWidth;
        if (containerHeight * scaleFactor > containerHeight) {
            scaleFactor = height() / containerHeight;
        }
    }
}