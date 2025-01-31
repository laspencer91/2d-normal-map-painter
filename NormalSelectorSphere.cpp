#include "NormalSelectorSphere.h"

#include <QMouseEvent>

NormalSelectorSphere::NormalSelectorSphere(QWidget *parent) : ::QOpenGLWidget(parent), currentNormal(0.0f, 0.0f, 1.0f) {
    updateNormal(0, 0, 0);
}

void NormalSelectorSphere::initializeGL() {
    initializeOpenGLFunctions();
}

QSize NormalSelectorSphere::sizeHint() const {
    int minSize = qMin(parentWidget()->width(), parentWidget()->height());
    minSize = qMax(minSize, 250);
    return QSize(minSize, minSize);  // Keep it square
}

void NormalSelectorSphere::resizeEvent(QResizeEvent *event) {
    int minSize = qMin(parentWidget()->width(), parentWidget()->height());
    minSize = qMax(minSize, 250);
    setFixedSize(minSize, minSize);  // Force a square size
    sphereDrawSize = std::min(width(), height()) - 20;
    update();
}

void NormalSelectorSphere::paintGL() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);
    painter.setBrush(Qt::gray);

    // Draw visualization ellipse, centered.
    const int centerX = width() / 2 - sphereDrawSize / 2;
    const int centerY = height() / 2 - sphereDrawSize / 2;
    painter.drawEllipse(centerX, centerY, sphereDrawSize, sphereDrawSize);

    // Convert normal (-1 to 1 range) to widget coordinates
    const int dotX = static_cast<int>(centerX + (currentNormal.x() * 0.5 + 0.5) * (sphereDrawSize));
    const int dotY = static_cast<int>(centerY + (-currentNormal.y() * 0.5 + 0.5) * (sphereDrawSize));

    // Draw the dot at the calculated position
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(QPoint(dotX, dotY), 6, 6);  // Small circle (radius 5)

    const int sampledX = static_cast<int>(centerX + (sampledNormal.x() * 0.5 + 0.5) * (sphereDrawSize));
    const int sampledY = static_cast<int>(centerY + (-sampledNormal.y() * 0.5 + 0.5) * (sphereDrawSize)); // Invert Y-axis

    painter.setBrush(QColor(30, 80, 210, 150));
    painter.drawEllipse(QPoint(sampledX, sampledY), 4, 4);  // Small circle (radius 5)
}

void NormalSelectorSphere::mouseMoveEvent(QMouseEvent *event) {
    updateNormal(event);
}

void NormalSelectorSphere::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        updateNormal(event);
    }
}

void NormalSelectorSphere::updateNormal(const QMouseEvent *event) {
    const float x = (event->position().x() - width() / 2.0f) / (width() / 2.0f);
    const float y = -(event->position().y() - height() / 2.0f) / (height() / 2.0f);
    const float z = sqrt(1.0f - qMin(1.0f, x * x + y * y));
    updateNormal(x, y, z);
}

void NormalSelectorSphere::updateNormal(const float x, const float y, const float z) {
    currentNormal = QVector3D(x, y, z).normalized();
    emit normalSelected(currentNormal);
    update();
}

void NormalSelectorSphere::onCanvasSampleUpdated(const QVector3D normal) {
    sampledNormal = normal;
    update();
}

void NormalSelectorSphere::onCanvasUpdatedNormal(const QVector3D normal) {
    currentNormal = normal;
    update();
}