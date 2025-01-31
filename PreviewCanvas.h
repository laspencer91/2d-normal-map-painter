#ifndef PREVIEWCANVAS_H
#define PREVIEWCANVAS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>

class PreviewCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit PreviewCanvas(QWidget *parent = nullptr);
    void setImages(QImage *image, QImage *normalMap);  // Load textures
    void setLightDirection(const QVector3D &direction);  // Set light direction

public slots:
    void onNormalMapPaintAdjusted();

protected:
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;;

private:
    QImage *baseImage;
    QImage *normalMap;
    QVector3D lightDirection;
    QPoint handlePosition;  // Position of the draggable light handle
    QRectF imageRect;  // Stores the scaled image position

    QPoint convertScreenToImage(int screenX, int screenY) const;
};

#endif // PREVIEWCANVAS_H