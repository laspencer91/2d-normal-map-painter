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
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void sampledNormalChanged(QVector3D normal);

private:
    QImage baseImage;           // The image being painted over.
    QImage normalMap;           // The image being painted.
    QVector3D currentNormal;    // Currently selected normal.
    QRectF imageRect;           // Scaled image position.
    QColor previousSampledColor;

    void applyPaint(int screenX, int screenY);
    void removePaint(int screenX, int screenY);
    void sampleNormalAt(int screenX, int screenY);
};

#endif // NORMALCANVAS_H
