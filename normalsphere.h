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
    void updateNormal(QMouseEvent *event);
};

#endif // NORMALSPHERE_H
