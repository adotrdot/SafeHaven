#ifndef SKETCH_H
#define SKETCH_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class Sketch: public QWidget
{
    Q_OBJECT

    public:
        Sketch(QWidget *parent = 0);

        void openImage(const QString &fileName);
        void saveImage(const QString &fileName);
        void setPenColor(const QColor &newColor);

        QColor getPenColor() const { return penColor; }

    public slots:
        void clearImage();

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        void drawLineTo(const QPoint &endPoint);
        void resizeImage(QImage *image, const QSize &newSize);

        bool scribbling = false;

        QColor penColor = Qt::black;
        int penWidth = 5;
        QImage image;
        QPoint lastPoint;
};

#endif
