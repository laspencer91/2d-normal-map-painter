#ifndef NORMALCANVAS_H
#define NORMALCANVAS_H

#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLWidget>

class NormalCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    NormalCanvas(QWidget *parent = nullptr);
    void loadImage(const QString &filePath);
    void saveNormalMap(const QString &filePath) const;

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent  * event) override;

signals:
    void sampledNormalChanged(QVector3D normal);
    void normalUpdated(QVector3D normal);

public slots:
    void onSphereSelectedNormal(const QVector3D &normal);

private:
    QImage baseImage;                 // The image being painted over.
    QImage normalMap;                 // The image being painted.
    QRectF imageRect;                 // Scaled image position.
    QVector3D currentNormal;          // Currently selected normal.
    QVector3D currentSampledNormal;
    QColor previousSampledColor;      // Track previous sampled color. Allows updating sample only on-change.
    float scaleFactor         = 1.0f; // Current scale of normal map
    float scaleAdjustmentStep = 0.1f; // How much to increase scale per mousewheel action.
    QPointF lastMousePos;              // Mouse position at previous frame.
    QPointF imagePan;

    void applyPaint(int screenX, int screenY);            // Paints at a point on the canvas.
    void removePaint(int screenX, int screenY);           // Erases paint from the Normal texture.
    void sampleNormalAt(int screenX, int screenY);        // Samples the normal at a position on the canvas.
    void fitImageInCanvas();                              // Sets the scale factor so the image fits in canvas.
    void setNormal(const QVector3D &normal);
    QPoint convertScreenToImage(int screenX, int screenY) const; // Convert canvas point to pixel location on image.
};

#endif // NORMALCANVAS_H
