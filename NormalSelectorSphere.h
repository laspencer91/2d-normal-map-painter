#ifndef NORMALSPHERE_H
#define NORMALSPHERE_H

#include <QMouseEvent>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>

class NormalSelectorSphere : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    NormalSelectorSphere(QWidget *parent = nullptr);

    signals:
        void normalSelected(QVector3D normal);

public slots:
    void onCanvasSampleUpdated(QVector3D normal);
    void onCanvasUpdatedNormal(QVector3D normal);

protected:
    void initializeGL() override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    int sphereDrawSize;                         // How large the visualization sphere is drawn
    QVector3D currentNormal;                    // Currently selected normal
    QVector3D sampledNormal;                    // From hovered pixel in the normal painting canvas.
    void updateNormal(const QMouseEvent *event);
    void updateNormal(float x, float y, float z);
};

#endif // NORMALSPHERE_H
