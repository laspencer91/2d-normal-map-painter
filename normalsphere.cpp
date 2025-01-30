#include "normalsphere.h"

#include <QMouseEvent>

NormalSphere::NormalSphere(QWidget *parent) : ::QOpenGLWidget(parent), currentNormal(0.0f, 0.0f, 1.0f) {}

void NormalSphere::initializeGL() {
    initializeOpenGLFunctions();
}

QSize NormalSphere::sizeHint() const {
    int minSize = qMin(parentWidget()->width(), parentWidget()->height());
    minSize = qMax(minSize, 250);
    return QSize(minSize, minSize);  // Keep it square
}

void NormalSphere::resizeEvent(QResizeEvent *event) {
    int minSize = qMin(parentWidget()->width(), parentWidget()->height());
    minSize = qMax(minSize, 250);
    setFixedSize(minSize, minSize);  // Force a square size
    sphereDrawSize = std::min(width(), height()) - 20;
    update();
}

void NormalSphere::paintGL() {
    QPainter painter(this);
    painter.setBrush(Qt::gray);

    // Draw visualization ellipse, centered.
    int centerX = width() / 2 - sphereDrawSize / 2;
    int centerY = height() / 2 - sphereDrawSize / 2;
    painter.drawEllipse(centerX, centerY, sphereDrawSize, sphereDrawSize);

    // Convert normal (-1 to 1 range) to widget coordinates
    int dotX = static_cast<int>(centerX + (currentNormal.x() * 0.5 + 0.5) * (sphereDrawSize));
    int dotY = static_cast<int>(centerY + (-currentNormal.y() * 0.5 + 0.5) * (sphereDrawSize));

    // Draw the dot at the calculated position
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(dotX, dotY), 5, 5);  // Small circle (radius 5)
}

void NormalSphere::mouseMoveEvent(QMouseEvent *event) {
    updateNormal(event);
}

void NormalSphere::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        updateNormal(event);
    }
}

void NormalSphere::updateNormal(QMouseEvent *event) {
    float x = (event->position().x() - width() / 2.0f) / (width() / 2.0f);
    float y = -(event->position().y() - height() / 2.0f) / (height() / 2.0f);
    float z = sqrt(1.0f - qMin(1.0f, x * x + y * y));

    currentNormal = QVector3D(x, y, z).normalized();
    emit normalChanged(currentNormal);
    update();
}