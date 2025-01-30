// normalcanvas.h
#ifndef NORMALCANVAS_H
#define NORMALCANVAS_H

#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLWidget>

class NormalCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    NormalCanvas(QWidget *parent = nullptr);
    void setNormal(const QVector3D &normal);
    void loadImage(const QString &filePath);

protected:
    void initializeGL() override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QImage baseImage;           // The image being painted over.
    QImage normalMap;           // The image being painted.
    QVector3D currentNormal;    // Currently selected normal.
    QRectF imageRect;           // Scaled image position.
};

#endif // NORMALCANVAS_H
