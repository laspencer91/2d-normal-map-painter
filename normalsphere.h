// normalsphere.h
#ifndef NORMALSPHERE_H
#define NORMALSPHERE_H

#include <QMouseEvent>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>

class NormalSphere : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    NormalSphere(QWidget *parent = nullptr);

    signals:
        void normalChanged(QVector3D normal);

public slots:
    void updateSampledNormal(QVector3D normal); // Connected to normalcanvas. Updates sampledNormal;

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
    void updateNormal(QMouseEvent *event);
};

#endif // NORMALSPHERE_H
